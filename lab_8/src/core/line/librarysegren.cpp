#include <QPainter>
#include "librarysegren.hpp"

void core::LibSegmentRenderer::draw(QImage& image, const Segment& segment, QColor color)
{
    QPainter painter(&image);
    painter.setPen(color);

    beginTiming();
    painter.drawLine(segment.x1, segment.y1, segment.x2, segment.y2);
    endTiming();
}
