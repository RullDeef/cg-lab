#pragma once

#include "primitiverenderer.hpp"

namespace core
{
    class LibraryCircleRenderer : public PrimitiveRenderer<Circle>
    {
    public:
        LibraryCircleRenderer(const char* name = u8"Библиотечный") : PrimitiveRenderer(name) {}

    protected:
        void draw(QImage& image, const Circle& circle, QColor color) override;
    };
}

