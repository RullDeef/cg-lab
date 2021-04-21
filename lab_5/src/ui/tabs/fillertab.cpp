#include "fillertab.hpp"

#include <QPen>
#include <QFormLayout>
#include <QObject>

using namespace core;
using namespace ui;


BasicRegion::Point& BasicRegionWrapper::appendPoint(int x, int y)
{
    BasicRegion::Point& point = region.appendPoint(x, y);
    emit pointAdded(point);
    return point;
}


CanvasRendererItem::CanvasRendererItem(QSize size)
    : image(QPixmap(size).toImage())
{
    image.convertTo(QImage::Format::Format_ARGB32);
    image.fill(Qt::white);
}

void CanvasRendererItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawImage(0, 0, image);
}

void CanvasRendererItem::resizeImage(QSize size)
{
    if (image.size() != size)
    {
        image = QPixmap(size).toImage();
        image.convertTo(QImage::Format::Format_ARGB32);
        image.fill(Qt::white);
    }
}


Canvas::Canvas()
    : canvasRenderer(new CanvasRendererItem)
{
    setScene(&scene);
    scene.addItem(canvasRenderer);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    canvasRenderer->resizeImage(size());
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
    canvasRenderer->resizeImage(size());

    scene.setSceneRect(QRectF(QPointF(0, 0), size()));
    scene.update();
}


void PointSelector::selectPoint(int x, int y)
{
    constexpr auto offset = 10;

    selectedPoint = nullptr;
    for (auto& point : (*region).getPoints())
    {
        bool x_meets = point.x - offset < x&& x < point.x + offset;
        bool y_meets = point.y - offset < y&& y < point.y + offset;
        if (x_meets && y_meets)
            selectedPoint = &point;
    }
}

void PointSelector::drawSelection(QPainter& painter) const
{
    if (selected())
    {
        QPen pen(Qt::red, 2);
        pen.setDashPattern({ 2, 2 });

        int x = (*selectedPoint).x;
        int y = (*selectedPoint).y;

        painter.drawEllipse(QPoint(x, y), 10, 10);
        painter.drawEllipse(QPoint(x, y), 8, 8);
    }
}


QPoint PointConstrainter::constraint(const QPoint& point)
{
    if (!enabled)
        return point;
    else
    {
        int offsetX = stickWidth;
        int offsetY = stickWidth;

        for (const auto& vert : (*region).getPoints())
        {
            bool x_meets = vert.x - stickWidth / 2 < point.x() && point.x() < vert.x + stickWidth / 2;
            bool y_meets = vert.y - stickWidth / 2 < point.y() && point.y() < vert.y + stickWidth / 2;

            if (x_meets ^ y_meets)
            {
                if (x_meets && std::abs(offsetX) > std::abs(vert.x - point.x()))
                    offsetX = vert.x - point.x();
                else if (y_meets && std::abs(offsetY) > std::abs(vert.y - point.y()))
                    offsetY = vert.y - point.y();
            }
        }

        if (offsetX == stickWidth) offsetX = 0;
        if (offsetY == stickWidth) offsetY = 0;

        return QPoint(point.x() + offsetX, point.y() + offsetY);
    }
}

void PointConstrainter::drawConstraints(QPainter& painter, const QPoint& point)
{
    if (enabled)
    {
        QPen pen(Qt::blue);
        pen.setDashPattern({ 5, 2 });

        painter.setPen(pen);

        for (const auto& vert : (*region).getPoints())
        {
            if (vert.x == point.x() || vert.y == point.y())
                painter.drawLine(QPoint(vert.x, vert.y), point);
        }
    }
}


SmartCanvas::SmartCanvas(BasicRegionWrapper& region)
    :  region(region), pointConstrainter(region), pointSelector(region)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);
    pointConstrainter.enable();
}

void SmartCanvas::regionModified()
{
    repaintCanvas();
}

void SmartCanvas::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_G)
        pointConstrainter.toggle();
}

void SmartCanvas::mouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();
    mousePos = pointConstrainter.constraint(mousePos);
    repaintCanvas();
}

void SmartCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::RightButton)
    {
        region.appendPoint(mousePos.x(), mousePos.y());
    }
    else if (event->button() == Qt::MouseButton::LeftButton)
    {
        auto oldSelected = pointSelector.getSelected();
        pointSelector.selectPoint(mousePos.x(), mousePos.y());

        if (oldSelected != pointSelector.getSelected())
            emit selectionChanged(pointSelector.getSelected());
    }

    repaintCanvas();
}

void SmartCanvas::repaintCanvas()
{
    QImage& image = getImage();
    image.fill(Qt::white);

    QPainter painter(&image);
    drawRegion(painter);

    pointSelector.drawSelection(painter);

    if (mouseEntered)
    {
        pointConstrainter.drawConstraints(painter, mousePos);
        drawCursor(painter);
    }

    repaint();
}

void SmartCanvas::drawRegion(QPainter& painter)
{
    painter.setPen(Qt::black);

    for (const auto& line : (*region).getLines())
        painter.drawLine(line.p1.x, line.p1.y, line.p2.x, line.p2.y);

    for (const auto& point : (*region).getPoints())
        painter.drawEllipse(QPoint(point.x, point.y), 2, 2);
}

void SmartCanvas::drawCursor(QPainter& painter)
{
    int x = mousePos.x();
    int y = mousePos.y();

    int padding = 3;
    int radius = 6;
    int margin = 5;

    painter.setPen(Qt::red);

    painter.drawEllipse(mousePos, radius, radius);
    painter.drawLine(x, y - padding, x, y - radius - margin);
    painter.drawLine(x, y + padding, x, y + radius + margin);
    painter.drawLine(x - padding, y, x - radius - margin, y);
    painter.drawLine(x + padding, y, x + radius + margin, y);
}


FillerTab::FillerTab(RegionRenderer* renderer)
    : InteractiveTabWidget(u8"заполнение"), renderer(renderer), regionWrapper(region)
{
    ui.setupUi(this);

    // setup canvas
    canvas = new SmartCanvas(regionWrapper);
    ui.canvasGroup->layout()->addWidget(canvas);

    hidePointEditor();

    QObject::connect(canvas, &SmartCanvas::selectionChanged, this, &FillerTab::selectionChangedSlot);
}

void FillerTab::selectionChangedSlot(core::BasicRegion::Point* selectedPoint)
{
    if (!selectedPoint)
        hidePointEditor();
    else
    {
        selectedPointWrapper.wrap(*selectedPoint);
        connect(&selectedPointWrapper, &PointWrapper::modified, canvas, &SmartCanvas::regionModified);

        showPointEditor();
    }
}

void FillerTab::showPointEditor()
{
    ui.pointEditorGroup->show();

    ui.xInput->setValue((*selectedPointWrapper).x);
    ui.yInput->setValue((*selectedPointWrapper).y);

    connect(ui.xInput, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, &selectedPointWrapper, &PointWrapper::setX);
    connect(ui.yInput, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, &selectedPointWrapper, &PointWrapper::setY);
}

void FillerTab::hidePointEditor()
{
    ui.pointEditorGroup->hide();
}
