#pragma once

#include "../regionrenderer.hpp"

namespace core
{
    class BoundRegionRenderer : public RegionRenderer
    {
    public:
        BoundRegionRenderer() : RegionRenderer(u8"���������� � ������������") {}

        void fill(QImage& image, const BasicRegion& region, QColor color);
    };
}
