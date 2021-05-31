#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class DDASegmentRenderer : public PrimitiveRenderer<Segment>
    {
    public:
        DDASegmentRenderer() : PrimitiveRenderer(u8"ÖÄÀ") {}

        void draw(QImage& image, const Segment& segment, QColor color) override;
    };
}
