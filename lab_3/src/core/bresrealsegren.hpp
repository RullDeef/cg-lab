#pragma once

#include "segmentrenderer.hpp"

namespace core
{
    class BresRealSegRen : public SegmentRenderer
    {
    public:
        BresRealSegRen(size_t queueSize = defaultQueueSize)
            : SegmentRenderer(u8"��������� ��������������", queueSize) {}

    protected:
        void drawClampedSegment(QImage& image, Segment& segment) override;
    };
}
