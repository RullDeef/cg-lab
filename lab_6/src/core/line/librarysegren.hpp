#include "../primitiverenderer.hpp"

namespace core
{
    class LibSegmentRenderer : public PrimitiveRenderer<Segment>
    {
    public:
        LibSegmentRenderer() : PrimitiveRenderer(u8"Библиотечный") {}

        void draw(QImage& image, const Segment& segment, QColor color) override;
    };
}
