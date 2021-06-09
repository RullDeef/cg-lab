#pragma once

#include "../basecutter.hpp"

namespace core
{
    class SimpleCutter : public BaseCutter
    {
    public:
        SimpleCutter() : BaseCutter(u8"�������") {}

        void cut(std::list<Segment>& lines, const Rect& cutter) override;
    };
}
