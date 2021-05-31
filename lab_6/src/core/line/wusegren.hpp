#include "../primitiverenderer.hpp"

namespace core
{
    class WuSegmentRenderer : public PrimitiveRenderer<Segment>
    {
    public:
        WuSegmentRenderer() : PrimitiveRenderer(u8"��") {}

        void draw(QImage& image, const Segment& segment, QColor color) override;
    };
}
