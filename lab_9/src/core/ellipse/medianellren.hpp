#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class MedianEllipseRenderer : public PrimitiveRenderer<Ellipse>
    {
    public:
        MedianEllipseRenderer(const char* name = u8"Средней точки") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Ellipse& circle, QColor color) override;
    };
}

