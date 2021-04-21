#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class CanonicEllipseRenderer : public PrimitiveRenderer<Ellipse>
    {
    public:
        CanonicEllipseRenderer(const char* name = u8"Канонический") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Ellipse& circle, QColor color) override;
    };
}

