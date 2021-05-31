#pragma once

#include "../primitiverenderer.hpp"

namespace core
{
    class BresRealSegmentRenderer : public PrimitiveRenderer<Segment>
    {
    public:
        BresRealSegmentRenderer() : PrimitiveRenderer(u8"��������� ��������������") {}

        void draw(QImage& image, const Segment& segment, QColor color) override;
    };
}
