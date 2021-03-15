#include "librarysegren.hpp"
#include <QPainter>

void core::LibSegRen::drawClampedSegment(QImage& image, Segment& segment)
{
    QPainter painter(&image);
    painter.setPen(segment.color);
    painter.beginNativePainting();

    startTiming();
    painter.drawLine(segment.x1, segment.y1, segment.x2, segment.y2);
    stopTiming(std::abs(segment.x2 - segment.x1) + 1,
        std::abs(segment.y2 - segment.y1) + 1);

    painter.endNativePainting();
}
