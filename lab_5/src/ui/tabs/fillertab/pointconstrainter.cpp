#include <QPainter>
#include "pointconstrainter.hpp"

using namespace ui;

QPoint PointConstrainter::constraint(const QPoint& point)
{
    if (!enabled)
        return point;
    else
    {
        int offsetX = stickWidth;
        int offsetY = stickWidth;

        for (const auto& vert : (*region).getPoints())
        {
            bool x_meets = vert.x - stickWidth / 2 < point.x() && point.x() < vert.x + stickWidth / 2;
            bool y_meets = vert.y - stickWidth / 2 < point.y() && point.y() < vert.y + stickWidth / 2;

            if (x_meets ^ y_meets)
            {
                if (x_meets && std::abs(offsetX) > std::abs(vert.x - point.x()))
                    offsetX = vert.x - point.x();
                else if (y_meets && std::abs(offsetY) > std::abs(vert.y - point.y()))
                    offsetY = vert.y - point.y();
            }
        }

        if (offsetX == stickWidth) offsetX = 0;
        if (offsetY == stickWidth) offsetY = 0;

        return QPoint(point.x() + offsetX, point.y() + offsetY);
    }
}

void PointConstrainter::drawConstraints(QPainter& painter, const QPoint& point)
{
    if (enabled)
    {
        QPen pen(Qt::blue);
        pen.setDashPattern({ 5, 2 });

        painter.setPen(pen);

        for (const auto& vert : (*region).getPoints())
        {
            if (vert.x == point.x() || vert.y == point.y())
                painter.drawLine(QPoint(vert.x, vert.y), point);
        }
    }
}
