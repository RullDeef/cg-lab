#pragma once

#include "basicregionwrapper.hpp"

namespace ui
{
    class PointSelector : public QObject
    {
        Q_OBJECT

    public:
        PointSelector(BasicRegionWrapper& region) : region(region) {}

        void selectPoint(int x, int y);
        void deselectPoint() { emit selectionChanged(selectedPoint, nullptr); selectedPoint = nullptr; }
        bool selected() const { return selectedPoint; }

        core::BasicRegion::Point* getSelected() { return selectedPoint; }

        bool removePointAt(int x, int y);

        void drawSelection(QPainter& painter) const;

    signals:
        void selectionChanged(core::BasicRegion::Point* oldSelectedPoint, core::BasicRegion::Point* newSelectedPoint);

    protected:
        core::BasicRegion::Point* pointAt(int x, int y);

    private:
        BasicRegionWrapper& region;
        core::BasicRegion::Point* selectedPoint = nullptr;
    };
}
