#pragma once

#include "primitiverenderer.hpp"

namespace core
{
    class BresenhemCircleRenderer : public PrimitiveRenderer<Circle>
    {
    public:
        BresenhemCircleRenderer(const char* name = u8"Брезенхэм") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Circle& circle, QColor color) override;
    };
}

