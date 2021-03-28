#include <QPainter>

#include "libcirren.hpp"

void LibCirRen::drawCircle(QImage& image, const Circle& circle)
{
    QPainter painter(&image);
    painter.setPen(circle.color);
    painter.beginNativePainting();

    startTiming();
    painter.drawEllipse(circle.x - circle.radius, circle.y - circle.radius, 2 * circle.radius, 2 * circle.radius);
    stopTiming(circle.radius);

    painter.endNativePainting();
}
