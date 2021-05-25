#pragma once

#include <list>
#include "../regionrenderer.hpp"

namespace core
{
    class AsyncBoundRegionRenderer : public AsyncRegionRenderer
    {
    public:
        AsyncBoundRegionRenderer() : AsyncRegionRenderer(u8"���������� � ������������") {}

        void fill(QImage& image, const BasicRegion& region, QColor color, int bound) override;
        void asyncFill(QImage& image, const BasicRegion& region, QColor color, int bound) override;

    private:
        bool initialized = false;
        int boundX = 0;
        int currentY;
        std::list<BasicRegion::Line>::const_iterator currentLine;
        BasicRegion currentRegion;
    };

    using BoundRegionRenderer = AsyncBoundRegionRenderer;
}
