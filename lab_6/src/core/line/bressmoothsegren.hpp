#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class BresSmoothSegmentRenderer : public PrimitiveRenderer<Segment>
    {
    public:
        BresSmoothSegmentRenderer() : PrimitiveRenderer(u8"Брезенхэм с устранением") {}

        void draw(QImage& image, const Segment& segment, QColor color) override;
    };
}
