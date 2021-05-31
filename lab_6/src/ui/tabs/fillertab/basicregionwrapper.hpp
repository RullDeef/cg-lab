#pragma once

#include "../../../core/regionrenderer.hpp"

namespace ui
{
    class PointWrapper : public QObject
    {
        Q_OBJECT

    public:
        PointWrapper() = default;
        PointWrapper(core::BasicRegion::Point& point) : point(&point) {}
        virtual ~PointWrapper() { disconnect(); }

        void wrap(core::BasicRegion::Point& point) { this->point = &point; }

        void setX(int x) { point->x = x; emit modified(); }
        void setY(int y) { point->y = y; emit modified(); }
        void setXY(int x, int y) { point->x = x; point->y = y; emit modified(); }

        core::BasicRegion::Point& operator*() { return *point; }

    signals:
        void modified();

    private:
        core::BasicRegion::Point* point = nullptr;
    };

    class BasicRegionWrapper : public QObject
    {
        Q_OBJECT

    public:
        BasicRegionWrapper(core::BasicRegion& region) : region(region) {}

        core::BasicRegion& operator*() { return region; }
        core::BasicRegion* operator->() { return &region; }

    signals:
        void pointAdded(core::BasicRegion::Point& point);

    public slots:
        core::BasicRegion::Point& appendPoint(int x, int y)
        {
            core::BasicRegion::Point& point = region.appendPoint(x, y);
            emit pointAdded(point);
            return point;
        }

    private:
        core::BasicRegion& region;
    };
}
