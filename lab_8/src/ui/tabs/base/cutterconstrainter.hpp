#pragma once

#include "core/regionrenderer.hpp"
#include <QPainter>


class CutterConstrainter
{
public:
    explicit CutterConstrainter(const core::Point& startPoint);
    core::Point constraint(const core::Point& p, const core::BasicRegion& region);

    void drawConstraints(QPainter& painter, const core::Point& p, const core::BasicRegion& region);

private:
    std::list<core::Line> get3Lines(const core::BasicRegion& region) const;

    const core::Point& startPoint;
};
