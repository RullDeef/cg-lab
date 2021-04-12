#pragma once

#include "primitiverenderer.hpp"

namespace core
{
    class BresenhemEllipseRenderer : public PrimitiveRenderer<Ellipse>
    {
    public:
        BresenhemEllipseRenderer(const char* name = u8"���������") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Ellipse& ellipse, QColor color) override;
    };
}

