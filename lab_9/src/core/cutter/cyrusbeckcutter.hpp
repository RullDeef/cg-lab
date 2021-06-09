#pragma once

#include "../basecutter.hpp"

namespace core
{
    class CyrusBeckCutter : public ConvexCutter
    {
    public:
        CyrusBeckCutter() : ConvexCutter(u8"Кируса-Бека") {}

        void cut(std::list<Segment>& lines, const BasicRegion& cutter) override;
    };
}
