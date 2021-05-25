#include "segmentrenderer.hpp"

inline void clampPoint(double width, double& x, double& y, double dx, double dy)
{
    if (x < 0.0)
    {
        y -= x * dy / dx;
        x = 0.0;
    }
    else if (x >= width)
    {
        y += (width - 1 - x) * dy / dx;
        x = width - 1;
    }
}

void core::SegmentRenderer::drawSegment(QImage& image, Segment segment)
{
    double width = image.width();
    double height = image.height();

    // clamp end points
    double dx = segment.x2 - segment.x1;
    double dy = segment.y2 - segment.y1;
    clampPoint(width, segment.x1, segment.y1, dx, dy);
    clampPoint(height, segment.y1, segment.x1, dy, dx);
    clampPoint(width, segment.x2, segment.y2, -dx, -dy);
    clampPoint(height, segment.y2, segment.x2, -dy, -dx);

    // call virtual method
    drawClampedSegment(image, segment);
}

void core::SegmentRenderer::startTiming()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void core::SegmentRenderer::stopTiming(double dx, double dy)
{
    if (!timing_active)
        return;

    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<durr_t>(endTime - startTime);

    drawTimes.push_front(elapsed.count() / 1000.0);
    if (drawTimes.size() > queueSize)
        drawTimes.pop_back();
}
