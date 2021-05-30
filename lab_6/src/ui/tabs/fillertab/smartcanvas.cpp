#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include "smartcanvas.hpp"

#include "../../../core/line/librarysegren.hpp"

using namespace core;
using namespace ui;

SmartCanvas::SmartCanvas()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SmartCanvas::rendererLoop);
}

SmartCanvas::~SmartCanvas()
{
    QObject::disconnect();

    if (asyncRenderer)
        delete asyncRenderer;
}

void SmartCanvas::specialPointSelectAction()
{
    specialPointSelectingState = true;
}

void SmartCanvas::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    default: break;
    }
}

void SmartCanvas::fillRegion(core::BucFillRenderer& renderer, const QColor& color)
{
    QPainter painter(&fillOverlay);
    drawRegion(painter);

    renderer.fill(fillOverlay, specialPointPos, color);
}

void SmartCanvas::fillRegionWithStep(core::AsyncBucFillRenderer* renderer, const QColor& color)
{
    this->color = color;

    if (asyncRenderer)
        delete asyncRenderer;

    QPainter painter(&fillOverlay);
    drawRegion(painter);

    asyncRenderer = renderer;
    asyncRenderer->beginFill();

    timer->start(30);
}

void SmartCanvas::clearCanvas()
{
    timer->stop();

    getImage().fill(Qt::white);
    fillOverlay.fill(Qt::white);

    if (asyncRenderer)
    {
        delete asyncRenderer;
        asyncRenderer = nullptr;
    }

    update();
}

void ui::SmartCanvas::clearRegion()
{
    region.clear();
    update();
}

void ui::SmartCanvas::clearOverlay()
{
    timer->stop();

    fillOverlay.fill(Qt::white);
    update();
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

    if (specialPointSelectingState)
        changeSpecialPointPos(mousePos.x(), mousePos.y());

    else if (mousePressed)
        region.back().push_back(mousePos);

    update();
}

void SmartCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        if (specialPointSelectingState)
            specialPointSelectingState = false;
        else
            region.push_back({ event->pos() });
    }
    else if (event->button() == Qt::MouseButton::RightButton)
    {
        if (specialPointSelectingState)
        {
            changeSpecialPointPos(0, 0);
            specialPointSelectingState = false;
        }
    }

    mousePressed = true;
    update();
}

void SmartCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    mousePressed = false;
    if (region.size() > 0)
        region.back().push_back(region.back().front());
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
            for (int i = 0; i < 5 && !asyncRenderer->isFinished(); i++)
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

void SmartCanvas::repaintCanvas()
{
    QImage& image = getImage();
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.drawImage(0, 0, fillOverlay);

    drawAxes(painter);
    drawRegion(painter);
    drawSpecialPoint(painter);

    if (mouseEntered)
        drawCursor(painter);

    repaint();
}

void SmartCanvas::drawRegion(QPainter& painter)
{
    for (const auto& contour : region)
    {
        if (contour.size() > 1)
        {
            painter.setPen(color);

            auto first = contour.begin();
            for (auto second = contour.begin(); ++second != contour.end();)
            {
                painter.drawLine(*first, *second);
                first = second;
            }
        }
    }
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

    constexpr int padding = 2;
    constexpr int radius = 4;
    constexpr int margin = 3;

    painter.setPen(Qt::blue);

    painter.drawEllipse(specialPointPos, radius, radius);
    painter.drawLine(x, y - padding, x, y - radius - margin);
    painter.drawLine(x, y + padding, x, y + radius + margin);
    painter.drawLine(x - padding, y, x - radius - margin, y);
    painter.drawLine(x + padding, y, x + radius + margin, y);
}
