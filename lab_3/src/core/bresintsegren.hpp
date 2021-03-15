#pragma once

#include "segmentrenderer.hpp"

namespace core
{
    class BresIntSegRen : public SegmentRenderer
    {
    public:
        BresIntSegRen(size_t queueSize = defaultQueueSize)
            : SegmentRenderer(u8"Брезенхэм целочисленный", queueSize) {}

    protected:
        void drawClampedSegment(QImage& image, Segment& segment) override;
    };
}
