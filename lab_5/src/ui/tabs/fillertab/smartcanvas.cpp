#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include "smartcanvas.hpp"

using namespace core;
using namespace ui;

SmartCanvas::SmartCanvas(BasicRegionWrapper& region)
    : region(region), pointConstrainter(region), pointSelector(region), lineConnector(region)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);
    pointConstrainter.disable();
    lineConnector.disable();

    connect(&pointSelector, &PointSelector::selectionChanged, this, &SmartCanvas::selectionChangedSlot);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SmartCanvas::rendererLoop);
}

SmartCanvas::~SmartCanvas()
{
    QObject::disconnect();

    if (asyncRenderer)
        delete asyncRenderer;
}

void SmartCanvas::regionModified()
{
    update();
}

void SmartCanvas::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_G: pointConstrainter.toggle(); emit constraintToggled(pointConstrainter.isEnabled()); break;
    case Qt::Key_L: lineConnector.toggle(); emit lineToggled(lineConnector.isEnabled()); break;
    default: break;
    }
}

long long SmartCanvas::fillRegion(core::RegionRenderer& renderer, const QColor& color)
{
    int bound = 0;

    for (const auto& line : region->getLines())
        bound += line.p1->x;
    bound /= region->getLines().size();

    renderer.fill(fillOverlay, *region, color, bound);
    return renderer.getDuration().count();
}

void SmartCanvas::fillRegionWithStep(core::AsyncRegionRenderer* renderer, const QColor& color)
{
    this->color = color;

    if (asyncRenderer)
        delete asyncRenderer;

    asyncRenderer = renderer;
    asyncRenderer->beginFill();

    timer->start(1);
}

void SmartCanvas::clearCanvas()
{
    timer->stop();

    pointSelector.deselectPoint();
    getImage().fill(Qt::white);
    fillOverlay.fill(Qt::white);

    if (asyncRenderer)
    {
        delete asyncRenderer;
        asyncRenderer = nullptr;
    }

    update();
}

void ui::SmartCanvas::clearOverlay()
{
    timer->stop();

    fillOverlay.fill(Qt::white);
    update();
}

void SmartCanvas::setConstraintEnabled(bool enabled)
{
    if (enabled ^ pointConstrainter.isEnabled())
        pointConstrainter.toggle();
}

void SmartCanvas::setLineEnabled(bool enabled)
{
    if (enabled ^ lineConnector.isEnabled())
        lineConnector.toggle();
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

void ui::SmartCanvas::resizeEvent(QResizeEvent* event)
{
    fillOverlay = QImage(width(), height(), QImage::Format_ARGB32);
    fillOverlay.fill(Qt::white);
}

void SmartCanvas::paintEvent(QPaintEvent* event)
{
    Canvas::paintEvent(event);
    repaintCanvas();
}

void ui::SmartCanvas::rendererLoop()
{
    if (asyncRenderer)
    {
        if (asyncRenderer->isFinished())
        {
            delete asyncRenderer;
            asyncRenderer = nullptr;
            timer->stop();
        }
        else
        {
            int bound = 0;
            for (const auto& line : region->getLines())
                bound += line.p1->x;
            bound /= region->getLines().size();

            for (int i = 0; i < 3; i++)
                asyncRenderer->asyncFill(fillOverlay, *region, color, bound);
            repaint();

            if (asyncRenderer->isFinished())
            {
                delete asyncRenderer;
                asyncRenderer = nullptr;
                timer->stop();
            }
        }
    }
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
    painter.drawImage(0, 0, fillOverlay);

    drawAxes(painter);
    drawRegion(painter);

    pointSelector.drawSelection(painter);
    lineConnector.drawHelpers(painter, mousePos);

    drawBound(painter);

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

    for (const auto& line : region->getLines())
        painter.drawLine(line.p1->x, line.p1->y, line.p2->x, line.p2->y);

    for (const auto& point : region->getPoints())
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

void ui::SmartCanvas::drawBound(QPainter& painter)
{
    if (region->getLines().size() == 0)
        return;

    int bound = 0;
    int yMin = size().height();
    int yMax = 0;
    constexpr int yOffset = 2;

    for (const auto& line : region->getLines())
    {
        bound += line.p1->x + line.p2->x;
        yMin = std::min(yMin, line.p1->y - yOffset);
        yMin = std::min(yMin, line.p2->y - yOffset);
        yMax = std::max(yMax, line.p1->y + yOffset);
        yMax = std::max(yMax, line.p2->y + yOffset);
    }
    bound /= 2 * region->getLines().size();

    painter.setPen(QPen(Qt::red, 1));
    painter.drawLine(bound, yMin, bound, yMax);
}
