#include "ellipserenderer.hpp"

void EllipseRenderer::startTiming()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void EllipseRenderer::stopTiming(double rx, double ry)
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<durr_t>(endTime - startTime);

    drawTimes.push_front(elapsed.count() / std::max(rx, ry) / 1000.0);
    if (drawTimes.size() > queueSize)
        drawTimes.pop_back();
}
