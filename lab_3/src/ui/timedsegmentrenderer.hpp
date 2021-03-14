#pragma once

#include <chrono>
#include "segmentrenderer.hpp"

namespace ui
{
    class TimedSegmentRenderer : public SegmentRenderer
    {
    public:
        std::chrono::microseconds timedRenderSegment(const Segment& segment);
    };
}
