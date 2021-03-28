#pragma once

#include "segmentrenderer.hpp"

namespace core
{
    class DDASegRen : public SegmentRenderer
    {
    public:
        DDASegRen(size_t queueSize = defaultQueueSize)
            : SegmentRenderer(u8"ждю", queueSize) {}

    protected:
        void drawClampedSegment(QImage& image, Segment& segment) override;
    };
}
