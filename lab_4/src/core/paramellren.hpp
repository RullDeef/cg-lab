#pragma once

#include "primitiverenderer.hpp"

namespace core
{
    class ParametricEllipseRenderer : public PrimitiveRenderer<Ellipse>
    {
    public:
        ParametricEllipseRenderer(const char* name = u8"Параметрический") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Ellipse& ellipse, QColor color) override;
    };
}

