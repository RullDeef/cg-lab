#pragma once

#include "../regionrenderer.hpp"

namespace core
{
    class BoundRegionRenderer : public RegionRenderer
    {
    public:
        BoundRegionRenderer() : RegionRenderer(u8"Заполнение с перегородкой") {}

        void fill(QImage& image, const BasicRegion& region, QColor color);
    };
}
