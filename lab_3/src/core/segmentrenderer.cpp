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
    // check for invalid/invisible segments
    double width = image.width();
    double height = image.height();

    // clamp end points
    double dx = segment.x2 - segment.x1;
    double dy = segment.y2 - segment.y1;
    // clampPoint(width, segment.x1, segment.y1, dx, dy);
    // clampPoint(height, segment.y1, segment.x1, dy, dx);
    // clampPoint(width, segment.x2, segment.y2, -dx, -dy);
    // clampPoint(height, segment.y2, segment.x2, -dy, -dx);
    
    if (segment.x1 < 0.0)
    {
        segment.y1 += (-segment.x1) * (segment.y2 - segment.y1) / (segment.x2 - segment.x1);
        segment.x1 = 0.0;
    }
    else if (segment.x1 >= width)
    {
        segment.y1 += (width - 1 - segment.x1) * (segment.y2 - segment.y1) / (segment.x2 - segment.x1);
        segment.x1 = width - 1;
    }

    if (segment.y1 < 0.0)
    {
        segment.x1 += (-segment.y1) * (segment.x2 - segment.x1) / (segment.y2 - segment.y1);
        segment.y1 = 0.0;
    }
    else if (segment.y1 >= height)
    {
        segment.x1 += (height - 1 - segment.y1) * (segment.x2 - segment.x1) / (segment.y2 - segment.y1);
        segment.y1 = height - 1;
    }

    if (segment.x2 < 0.0)
    {
        segment.y2 += (-segment.x2) * (segment.y1 - segment.y2) / (segment.x1 - segment.x2);
        segment.x2 = 0.0;
    }
    else if (segment.x2 >= width)
    {
        segment.y2 += (width - 1 - segment.x2) * (segment.y1 - segment.y2) / (segment.x1 - segment.x2);
        segment.x2 = width - 1;
    }

    if (segment.y2 < 0.0)
    {
        segment.x2 += (-segment.y2) * (segment.x1 - segment.x2) / (segment.y1 - segment.y2);
        segment.y2 = 0.0;
    }
    else if (segment.y2 >= height)
    {
        segment.x2 += (height - 1 - segment.y2) * (segment.x1 - segment.x2) / (segment.y1 - segment.y2);
        segment.y2 = height - 1;
    }
    

    // call virtual method
    drawClampedSegment(image, segment);
}

void core::SegmentRenderer::startTiming()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void core::SegmentRenderer::stopTiming()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<durr_t>(endTime - startTime);

    drawTimes.push_front(elapsed);
    if (drawTimes.size() > queueSize)
        drawTimes.pop_back();
}
