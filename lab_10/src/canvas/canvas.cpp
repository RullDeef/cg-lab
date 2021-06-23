#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include "canvas.hpp"


Canvas::Canvas()
    : renderTarget(QImage(100, 100, QImage::Format::Format_ARGB32)), floatingHorizon(1)
{
    renderTarget.fill(Qt::transparent);
    floatingHorizon.setStep(1);
}

void Canvas::setSurface(Y_t function)
{
    floatingHorizon.setSurface([=](double x, double z) {
        double yf = function(x, z);
        return (end.getY() - yf) / (end.getY() - start.getY()) * height();
    });
}

void Canvas::setViewport(double startX, double endX, double startY, double endY, double startZ, double endZ)
{
    start = Vector(startX, startY, startZ);
    end = Vector(endX, endY, endZ);
}

void Canvas::setCounts(size_t xCount, size_t zCount, size_t nTicks)
{
    floatingHorizon.setViewport(start, end, xCount, zCount, nTicks);
}

void Canvas::rotateView(double angleX, double angleY, double angleZ)
{
    floatingHorizon.rotateView(angleX, angleY, angleZ);
}

void Canvas::resetRotation()
{
    floatingHorizon.resetRotation();
}

void Canvas::drawSurface()
{
    QPainter painter(&renderTarget);
    floatingHorizon.draw(painter);
}

void Canvas::clear()
{
    renderTarget.fill(Qt::transparent);
    update();
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, renderTarget);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();

    QImage oldRenderTarget = renderTarget;
    renderTarget = QImage(size.width(), size.height(), QImage::Format::Format_ARGB32);
    renderTarget.fill(Qt::transparent);

    QPainter painter(&renderTarget);
    painter.drawImage(0, 0, oldRenderTarget);
    floatingHorizon.setWidth(size.width());
    floatingHorizon.draw(painter);

    update();
}
