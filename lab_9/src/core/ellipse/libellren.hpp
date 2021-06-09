#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class LibraryEllipseRenderer : public PrimitiveRenderer<Ellipse>
    {
    public:
        LibraryEllipseRenderer(const char* name = u8"Библиотечный") : PrimitiveRenderer(name) {}

        void draw(QImage& image, const Ellipse& ellipse, QColor color) override;
    };
}

