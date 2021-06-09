#pragma once

#include "core/regionrenderer.hpp"


namespace core
{
    class BasicRegionBuilder
    {
    public:
        explicit BasicRegionBuilder(double mergeDistance = 10.0);

        void updateLastPoint(int x, int y);
        void confirmLastPoint();

        void reset();

        BasicRegion getResult();

    private:
        Point tempPoint;
        std::list<Point> points;
        std::list<Line> contour;
        std::list<std::list<Line>> contours;

        double mergeDistance;
    };
}
