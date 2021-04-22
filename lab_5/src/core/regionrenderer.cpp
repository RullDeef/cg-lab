#pragma once

#include <algorithm>
#include "regionrenderer.hpp"

using namespace core;

QRectF BasicRegion::getBoundingBox() const
{
    QRectF rect(points.front().x, points.front().y, 0, 0);

    for (const auto& point : points)
    {
        rect.setLeft(std::min(rect.left(), qreal(point.x)));
        rect.setRight(std::max(rect.right(), qreal(point.x)));
        rect.setTop(std::min(rect.top(), qreal(point.y)));
        rect.setBottom(std::max(rect.bottom(), qreal(point.y)));
    }

    return rect;
}
