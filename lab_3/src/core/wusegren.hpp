#include "segmentrenderer.hpp"

namespace core
{
    class WuSegRen : public SegmentRenderer
    {
    public:
        WuSegRen(size_t queueSize = defaultQueueSize)
            : SegmentRenderer(u8"��", queueSize) {}

    protected:
        void drawClampedSegment(QImage& image, Segment& segment) override;
    };
}
