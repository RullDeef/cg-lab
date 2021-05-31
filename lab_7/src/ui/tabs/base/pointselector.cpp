#include <QPainter>
#include "pointselector.hpp"

using namespace ui;


void PointSelector::selectPoint(int x, int y)
{
    core::BasicRegion::Point* oldSelected = selectedPoint;
    selectedPoint = pointAt(x, y);

    if (oldSelected != selectedPoint)
        emit selectionChanged(oldSelected, selectedPoint);
}

bool PointSelector::removePointAt(int x, int y)
{
    auto point = pointAt(x, y);

    if (point)
    {
        if (selectedPoint != nullptr && *point == *selectedPoint)
            deselectPoint();

        (*region).removePoint(*point);
        return true;
    }

    return false;
}

void PointSelector::drawSelection(QPainter& painter) const
{
    if (selected())
    {
        QPen pen(Qt::red, 2);
        pen.setDashPattern({ 2, 2 });

        int x = (*selectedPoint).x;
        int y = (*selectedPoint).y;

        painter.drawEllipse(QPoint(x, y), 10, 10);
        painter.drawEllipse(QPoint(x, y), 8, 8);
    }
}

core::BasicRegion::Point* PointSelector::pointAt(int x, int y)
{
    constexpr auto offset = 10;

    for (auto& point : (*region).getPoints())
    {
        bool x_meets = point.x - offset < x&& x < point.x + offset;
        bool y_meets = point.y - offset < y&& y < point.y + offset;
        if (x_meets && y_meets)
            return &point;
    }

    return nullptr;
}
