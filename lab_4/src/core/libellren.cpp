#include <QPainter>

#include "libellren.hpp"

void LibEllRen::drawEllipse(QImage& image, const Ellipse& ellipse)
{
    QPainter painter(&image);
    painter.setPen(ellipse.color);
    painter.beginNativePainting();

    startTiming();
    painter.drawEllipse(ellipse.x - ellipse.width / 2, ellipse.y - ellipse.height / 2, ellipse.width, ellipse.height);
    stopTiming(ellipse.width, ellipse.height);

    painter.endNativePainting();
}
