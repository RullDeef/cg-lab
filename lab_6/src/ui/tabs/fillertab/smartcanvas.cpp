#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include "smartcanvas.hpp"

#include "../../../core/line/librarysegren.hpp"

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

void SmartCanvas::specialPointSelectAction()
{
    deselect();
    specialPointSelectingState = true;
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

void SmartCanvas::fillRegion(core::BucFillRenderer& renderer, const QColor& color)
{
    core::LibSegmentRenderer segren;
    for (const auto line : region->getLines())
        segren.draw(fillOverlay, line, color);
    renderer.fill(fillOverlay, specialPointPos, color);
}

void SmartCanvas::fillRegionWithStep(core::AsyncBucFillRenderer* renderer, const QColor& color)
{
    this->color = color;

    if (asyncRenderer)
        delete asyncRenderer;

    core::LibSegmentRenderer segren;
    for (const auto line : region->getLines())
        segren.draw(fillOverlay, line, color);

    asyncRenderer = renderer;
    asyncRenderer->beginFill();

    timer->start(30);
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

void SmartCanvas::changeSpecialPointPos(int x, int y)
{
    if (specialPointPos != QPoint(x, y))
    {
        specialPointPos = QPoint(x, y);
        emit specialPointPosChanged(x, y);
    }
}

void SmartCanvas::mouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();
    mousePos = pointConstrainter.constraint(mousePos);

    if (specialPointSelectingState)
        changeSpecialPointPos(mousePos.x(), mousePos.y());

    update();
}

void SmartCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        if (specialPointSelectingState)
            specialPointSelectingState = false;
        else
            pointSelector.selectPoint(mousePos.x(), mousePos.y());
    }
    else if (event->button() == Qt::MouseButton::RightButton)
    {
        if (specialPointSelectingState)
        {
            changeSpecialPointPos(0, 0);
            specialPointSelectingState = false;
        }
        else if (QApplication::queryKeyboardModifiers().testFlag(Qt::KeyboardModifier::AltModifier))
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
            asyncRenderer->asyncFill(fillOverlay, specialPointPos, color);
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

    drawSpecialPoint(painter);

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

void SmartCanvas::drawSpecialPoint(QPainter& painter)
{
    int x = specialPointPos.x();
    int y = specialPointPos.y();

    int padding = 2;
    int radius = 4;
    int margin = 3;

    painter.setPen(Qt::blue);

    painter.drawEllipse(specialPointPos, radius, radius);
    painter.drawLine(x, y - padding, x, y - radius - margin);
    painter.drawLine(x, y + padding, x, y + radius + margin);
    painter.drawLine(x - padding, y, x - radius - margin, y);
    painter.drawLine(x + padding, y, x + radius + margin, y);
}
