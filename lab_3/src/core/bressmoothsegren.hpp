#pragma once

#include "segmentrenderer.hpp"

namespace core
{
    class BresSmoothSegRen : public SegmentRenderer
    {
    public:
        BresSmoothSegRen(size_t queueSize = defaultQueueSize)
            : SegmentRenderer(u8"��������� � �����������", queueSize) {}

    protected:
        void drawClampedSegment(QImage& image, Segment& segment) override;
    };
}
