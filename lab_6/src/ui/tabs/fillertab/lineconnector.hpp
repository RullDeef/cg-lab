#pragma once

#include "basicregionwrapper.hpp"

namespace ui
{
    class LineConnector : public QObject
    {
        Q_OBJECT

    public:
        LineConnector(BasicRegionWrapper& region) : region(region) {}

        void enable() { enabled = true; }
        void disable() { enabled = false; }
        void toggle() { enabled = !enabled; }

        bool isEnabled() const { return enabled; }

        bool removeLineAt(int x, int y);

        void drawHelpers(QPainter& painter, const QPoint& mousePos) const {}

    public slots:
        void selectionChanged(core::BasicRegion::Point* oldSelectedPoint, core::BasicRegion::Point* newSelectedPoint);

    private:
        bool enabled = true;
        BasicRegionWrapper& region;
    };
}
