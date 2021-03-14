#include "timedsegmentrenderer.hpp"

using namespace std::chrono;

microseconds ui::TimedSegmentRenderer::timedRenderSegment(const Segment& segment)
{
    auto start = high_resolution_clock::now();

    for (int i = 0; i < 1e6; i++)
    {
        i += i - (i + 1) + 2 - 1;
    }

    auto end = high_resolution_clock::now();

    return duration_cast<microseconds>(end - start);
}
