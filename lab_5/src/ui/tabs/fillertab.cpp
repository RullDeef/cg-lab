#include "fillertab.hpp"

#include <QPen>
#include <QFormLayout>
#include <QObject>

using namespace core;
using namespace ui;


ui::PointWrapper::~PointWrapper()
{
    disconnect();
}

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
    core::BasicRegion::Point* oldSelected = selectedPoint;
    selectedPoint = pointAt(x, y);

    if (oldSelected != selectedPoint)
        emit selectionChanged(oldSelected, selectedPoint);
}

bool PointSelector::removePointAt(int x, int y)
{
    auto point = pointAt(x, y);

    if (point)
    {
        if (selectedPoint != nullptr && *point == *selectedPoint)
            deselectPoint();

        (*region).removePoint(*point);
        return true;
    }

    return false;
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

core::BasicRegion::Point* PointSelector::pointAt(int x, int y)
{
    constexpr auto offset = 10;

    for (auto& point : (*region).getPoints())
    {
        bool x_meets = point.x - offset < x&& x < point.x + offset;
        bool y_meets = point.y - offset < y&& y < point.y + offset;
        if (x_meets && y_meets)
            return &point;
    }

    return nullptr;
}


bool LineConnector::removeLineAt(int x, int y)
{
    constexpr auto offset = 12.0;

    auto& lines = (*region).getLines();

    auto iter = std::find_if(lines.begin(), lines.end(),
        [x, y, offset](const core::BasicRegion::Line& line)
            { return line.closeTo(x, y, offset); });

    if (iter != lines.end())
    {
        lines.erase(iter);
        return true;
    }

    return false;
}

void LineConnector::selectionChanged(core::BasicRegion::Point* oldSelectedPoint, core::BasicRegion::Point* newSelectedPoint)
{
    if (!enabled)
        return;

    if (oldSelectedPoint == nullptr || newSelectedPoint == nullptr)
        return;

    (*region).connectPoints(*oldSelectedPoint, *newSelectedPoint);
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
    :  region(region), pointConstrainter(region), pointSelector(region), lineConnector(region)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);
    pointConstrainter.enable();
    lineConnector.disable();

    connect(&pointSelector, &PointSelector::selectionChanged, this, &SmartCanvas::selectionChangedSlot);
}

void SmartCanvas::regionModified()
{
    update();
}

void SmartCanvas::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_G: pointConstrainter.toggle(); break;
        case Qt::Key_L: lineConnector.toggle(); break;
        default: break;
    }
}

SmartCanvas::~SmartCanvas()
{
    QObject::disconnect();
}

void SmartCanvas::clearCanvas()
{
    pointSelector.deselectPoint();
    getImage().fill(Qt::white);
    update();
}

void SmartCanvas::mouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();
    mousePos = pointConstrainter.constraint(mousePos);
    update();
}

void SmartCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
        pointSelector.selectPoint(mousePos.x(), mousePos.y());
    else if (event->button() == Qt::MouseButton::RightButton)
    {
        if (QApplication::queryKeyboardModifiers().testFlag(Qt::KeyboardModifier::AltModifier))
        {
            if (!pointSelector.removePointAt(mousePos.x(), mousePos.y()))
                lineConnector.removeLineAt(mousePos.x(), mousePos.y());
        }
        else
        {
            region.appendPoint(mousePos.x(), mousePos.y());
            pointSelector.selectPoint(mousePos.x(), mousePos.y());
        }
    }

    update();
}

void SmartCanvas::paintEvent(QPaintEvent* event)
{
    Canvas::paintEvent(event);
    repaintCanvas();
}

void SmartCanvas::selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint)
{
    lineConnector.selectionChanged(oldPoint, newPoint);
    emit selectionChanged(oldPoint, newPoint);
}

void SmartCanvas::repaintCanvas()
{
    QImage& image = getImage();
    image.fill(Qt::white);

    QPainter painter(&image);

    drawAxes(painter);
    drawRegion(painter);

    pointSelector.drawSelection(painter);
    lineConnector.drawHelpers(painter, mousePos);

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
        painter.drawLine(line.p1->x, line.p1->y, line.p2->x, line.p2->y);

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

void SmartCanvas::drawAxes(QPainter& painter)
{
    QImage& image = getImage();

    for (int x = 0; x < image.width(); x += 25)
    {
        painter.drawLine(x, 0, x, x % 100 == 0 ? 10 : 6);
        if (x % 100 == 0)
            painter.drawText(x + 4, 10, QString::number(x));
    }

    for (int y = 0; y < image.height(); y += 25)
    {
        painter.drawLine(0, y, y % 100 == 0 ? 10 : 6, y);
        if (y % 100 == 0)
            painter.drawText(4, y, QString::number(y));
    }
}


FillerTab::FillerTab(RegionRenderer* renderer)
    : InteractiveTabWidget(u8"заполнение"), renderer(renderer), regionWrapper(region)
{
    ui.setupUi(this);

    // setup canvas
    canvas = new SmartCanvas(regionWrapper);
    ui.canvasGroup->layout()->addWidget(canvas);

    hidePointEditor();

    connect(canvas, &SmartCanvas::selectionChanged, this, &FillerTab::selectionChangedSlot);

    // clear canvas button
    connect(ui.clearButton, &QPushButton::clicked, this, &FillerTab::clearButtonPressed);
}

void FillerTab::selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint)
{
    if (!newPoint)
        hidePointEditor();
    else
    {
        selectedPointWrapper.wrap(*newPoint);
        connect(&selectedPointWrapper, &PointWrapper::modified, canvas, &SmartCanvas::regionModified);

        showPointEditor();
    }
}

void FillerTab::clearButtonPressed()
{
    canvas->clearCanvas();
    (*regionWrapper).clear();
}

void FillerTab::showPointEditor()
{
    ui.pointEditorGroup->show();

    ui.xInput->disconnect();
    ui.yInput->disconnect();

    ui.xInput->setValue((*selectedPointWrapper).x);
    ui.yInput->setValue((*selectedPointWrapper).y);

    connect(ui.xInput, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, &selectedPointWrapper, &PointWrapper::setX);
    connect(ui.yInput, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, &selectedPointWrapper, &PointWrapper::setY);
}

void FillerTab::hidePointEditor()
{
    ui.pointEditorGroup->hide();
}
