#pragma once

#include "primitiverenderer.hpp"

namespace core
{
    class CanonicCircleRenderer : public PrimitiveRenderer<Circle>
    {
    public:
        CanonicCircleRenderer(const char* name = u8"Канонический") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Circle& circle, QColor color) override;
    };
}

