#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class BresIntSegmentRenderer : public PrimitiveRenderer<Segment>
    {
    public:
        BresIntSegmentRenderer() : PrimitiveRenderer(u8"Брезенхэм целочисленный") {}

        void draw(QImage& image, const Segment& segment, QColor color) override;
    };
}
