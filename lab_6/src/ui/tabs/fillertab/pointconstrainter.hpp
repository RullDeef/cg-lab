#pragma once

#include "basicregionwrapper.hpp"

namespace ui
{
    class PointConstrainter
    {
    public:
        PointConstrainter(BasicRegionWrapper& region) : region(region) {}

        void enable() { enabled = true; }
        void disable() { enabled = false; }
        void toggle() { enabled = !enabled; }

        bool isEnabled() const { return enabled; }

        QPoint constraint(const QPoint& point);

        void drawConstraints(QPainter& painter, const QPoint& point);

    private:
        BasicRegionWrapper& region;

        int stickWidth = 12;
        bool enabled = true;
    };
}
