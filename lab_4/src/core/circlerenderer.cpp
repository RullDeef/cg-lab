#include "circlerenderer.hpp"

void CircleRenderer::startTiming()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void CircleRenderer::stopTiming(double r)
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<durr_t>(endTime - startTime);

    drawTimes.push_front(elapsed.count() / r / 1000.0);
    if (drawTimes.size() > queueSize)
        drawTimes.pop_back();
}
