#include "segmentrenderer.hpp"

namespace core
{
    class LibSegRen : public SegmentRenderer
    {
    public:
        LibSegRen(size_t queueSize = defaultQueueSize) :
            SegmentRenderer(u8"������������", queueSize) {}

    protected:
        void drawClampedSegment(QImage& image, Segment& segment) override;
    };
}
