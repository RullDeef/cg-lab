#include "BasicRegionBuilder.hpp"

using namespace core;


BasicRegionBuilder::BasicRegionBuilder(double mergeDistance)
    : mergeDistance(mergeDistance), tempPoint(0, 0)
{
}

void core::BasicRegionBuilder::updateLastPoint(int x, int y)
{
    if (contour.size() == 0)
    {
        tempPoint.x = x;
        tempPoint.y = y;
    }
    else
    {
        points.back().x = x;
        points.back().y = y;
    }
}

void core::BasicRegionBuilder::confirmLastPoint()
{
    if (contour.size() == 0)
    {
        points.push_back(tempPoint);
        points.push_back(tempPoint);

        auto iter_prev = points.end(); std::advance(iter_prev, -2);
        auto iter_last = points.end(); std::advance(iter_last, -1);
        contour.push_back(Line(*iter_prev, *iter_last));
    }
    else
    {
        auto iter_prev = points.end(); std::advance(iter_prev, -2);
        auto iter_last = points.end(); std::advance(iter_last, -1);
        if (iter_last->distance(*contour.front().p1) < mergeDistance)
        {
            points.pop_back();
            contour.back().p2 = contour.front().p1;
            contours.push_back(contour);
            contour = {};
        }
        else
        {
            points.push_back(points.back());
            contour.push_back(Line(*iter_last, points.back()));
        }
    }
}

void core::BasicRegionBuilder::reset()
{
    points.clear();
    contour.clear();
    contours.clear();
}

BasicRegion BasicRegionBuilder::getResult()
{
    BasicRegion region;

    for (const auto& contour : contours)
    {
        for (const auto& line : contour)
            region.appendPoint(line.p1->x, line.p1->y);

        for (const auto& line : contour)
        {
            Point p1(line.p1->x, line.p1->y);
            Point p2(line.p2->x, line.p2->y);
            region.connectPoints(p1, p2);
        }
    }

    if (contour.size() > 0)
    {
        const Point* p2 = &region.appendPoint(contour.front().p1->x, contour.front().p1->y);
        for (const auto& line : contour)
        {
            const Point* p1 = p2;
            p2 = &region.appendPoint(line.p2->x, line.p2->y);

            region.connectPoints(*p1, *p2);
        }
    }

    return region;
}
