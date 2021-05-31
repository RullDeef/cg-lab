#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class ParametricCircleRenderer : public PrimitiveRenderer<Circle>
    {
    public:
        ParametricCircleRenderer(const char* name = u8"Параметрический") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Circle& circle, QColor color) override;
    };
}

