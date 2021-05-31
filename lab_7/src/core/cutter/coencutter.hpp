#pragma once

#include "../basecutter.hpp"

namespace core
{
    class CoenCutter : public BaseCutter
    {
    public:
        CoenCutter() : BaseCutter(u8"Коэна-Сазерленда") {}

        void cut(std::list<Segment>& lines, const Rect& cutter) override;
    };
}
