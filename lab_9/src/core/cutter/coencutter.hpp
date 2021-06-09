#pragma once

#include "../basecutter.hpp"

namespace core
{
    class CoenCutter : public BaseCutter
    {
    public:
        CoenCutter() : BaseCutter(u8"Сазерленда-Коэна") {}

        void cut(std::list<Segment>& lines, const Rect& cutter) override;
    };
}
