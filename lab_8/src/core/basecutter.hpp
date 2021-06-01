#pragma once

#include "primitiverenderer.hpp"
#include "regionrenderer.hpp"
#include <list>

namespace core
{
    constexpr auto BIT_LEFT = 1 << 0;
    constexpr auto BIT_RIGHT = 1 << 1;
    constexpr auto BIT_BOTTOM = 1 << 2;
    constexpr auto BIT_TOP = 1 << 3;

    struct LinePatch
    {
        int p1x;
        int p1y;
        int p2x;
        int p2y;

        LinePatch(const Segment& seg)
            : p1x(seg.x1), p1y(seg.y1), p2x(seg.x2), p2y(seg.y2)
        {}

        operator Segment() const
        {
            Segment seg;
            seg.x1 = p1x; seg.y1 = p1y; seg.x2 = p2x; seg.y2 = p2y;
            return seg;
        }
    };

    class BaseCutter : public NamingStrategy, public TimingStrategy
    {
    public:
        BaseCutter(const char* name) : NamingStrategy(name) {}

        virtual void cut(std::list<Segment>& lines, const Rect& cutter) = 0;
    };

    class ConvexCutter : public NamingStrategy, public TimingStrategy
    {
    public:
        ConvexCutter(const char* name) : NamingStrategy(name) {}
        virtual ~ConvexCutter() = default;
        virtual void cut(std::list<Segment>& lines, const BasicRegion& cutter) = 0;
    };
}
