#pragma once

#include <algorithm>
#include "regionrenderer.hpp"

using namespace core;

std::vector<BasicRegion::Line> BasicRegion::getLines() const
{
    std::vector<Line> lines;

    if (points.size() > 1)
        for (auto first = points.cbegin(), second = points.cbegin() + 1; second != points.end(); first++, second++)
            lines.push_back(Line(*first, *second));

    return lines;
}

QRectF BasicRegion::getBoundingBox() const
{
    QRectF rect(points[0].x, points[0].y, 0, 0);

    for (const auto& point : points)
    {
        rect.setLeft(std::min(rect.left(), qreal(point.x)));
        rect.setRight(std::max(rect.right(), qreal(point.x)));
        rect.setTop(std::min(rect.top(), qreal(point.y)));
        rect.setBottom(std::max(rect.bottom(), qreal(point.y)));
    }

    return rect;
}
