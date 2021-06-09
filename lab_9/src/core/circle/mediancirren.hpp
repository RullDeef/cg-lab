#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class MedianCircleRenderer : public PrimitiveRenderer<Circle>
    {
    public:
        MedianCircleRenderer(const char* name = u8"Средней точки") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Circle& circle, QColor color) override;
    };
}

