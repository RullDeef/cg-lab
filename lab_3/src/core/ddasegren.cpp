#include "ddasegren.hpp"

void core::DDASegRen::drawClampedSegment(QImage& image, Segment& segment)
{
    QRgb color = segment.color.rgba();
    startTiming();

    if (segment.x2 < segment.x1) // swap end points
    {
        std::swap(segment.x1, segment.x2);
        std::swap(segment.y1, segment.y2);
    }

    double x = segment.x1;
    double y = segment.y1;

    int deltaX = std::abs(segment.x1 - segment.x2) + 1;
    int deltaY = std::abs(segment.y1 - segment.y2) + 1;
    unsigned int L = std::floor(std::hypot(deltaX, deltaY) + 1);

    double dx = (segment.x2 - segment.x1) / L;
    double dy = (segment.y2 - segment.y1) / L;

    for (unsigned int i = 0; i < L; i++)
    {
        image.setPixel(std::round(x), std::round(y), color);
        x += dx;
        y += dy;
    }

    stopTiming(deltaX, deltaY);
}
