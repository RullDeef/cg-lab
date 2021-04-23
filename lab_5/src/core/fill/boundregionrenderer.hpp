#pragma once

#include <list>
#include "../regionrenderer.hpp"

namespace core
{
    class AsyncBoundRegionRenderer : public AsyncRegionRenderer
    {
    public:
        AsyncBoundRegionRenderer() : AsyncRegionRenderer(u8"Заполнение с перегородкой") {}

        void fill(QImage& image, const BasicRegion& region, QColor color) override;
        void asyncFill(QImage& image, const BasicRegion& region, QColor color) override;

    private:
        bool initialized = false;
        int boundX = 0;
        int currentY;
        std::list<BasicRegion::Line>::const_iterator currentLine;
        BasicRegion currentRegion;
    };

    using BoundRegionRenderer = AsyncBoundRegionRenderer;
}
