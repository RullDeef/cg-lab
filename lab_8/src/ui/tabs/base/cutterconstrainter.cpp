#include <algorithm>
#include <vector>
#include "cutterconstrainter.hpp"


CutterConstrainter::CutterConstrainter(const core::Point& startPoint)
    : startPoint(startPoint)
{
}

core::Point CutterConstrainter::constraint(const core::Point& p, const core::BasicRegion& region)
{
    core::Point res = p;

    for (const auto& line : get3Lines(region))
    {
        double dx = line.p2->x - line.p1->x;
        double dy = line.p2->y - line.p1->y;
        double len = 300;

        double x1 = startPoint.x - len * dx;
        double y1 = startPoint.y - len * dy;
        double x2 = startPoint.x + len * dx;
        double y2 = startPoint.y + len * dy;

        core::Point p1(x1, y1), p2(x2, y2);
        core::Line dir(p1, p2);

        double dist = dir.getDistance(res.x, res.y);
        if (dist < 10)
        {
            double lem = core::Line(startPoint, res).length();
            lem = std::sqrt(lem * lem - dist * dist) / std::hypot(dx, dy);

            if (dx * (p.x - startPoint.x) + dy * (p.y - startPoint.y) > 0)
            {
                res.x = startPoint.x + lem * dx;
                res.y = startPoint.y + lem * dy;
            }
            else
            {
                res.x = startPoint.x - lem * dx;
                res.y = startPoint.y - lem * dy;
            }
        }
    }

    return res;
}

void CutterConstrainter::drawConstraints(QPainter& painter, const core::Point& p, const core::BasicRegion& region)
{
    QPen pen(Qt::cyan);
    pen.setDashPattern({ 5, 2 });
    painter.setPen(pen);

    for (const auto& line : get3Lines(region))
    {
        double dx = line.p2->x - line.p1->x;
        double dy = line.p2->y - line.p1->y;
        double len = 300;

        double x1 = startPoint.x - len * dx;
        double y1 = startPoint.y - len * dy;
        double x2 = startPoint.x + len * dx;
        double y2 = startPoint.y + len * dy;

        painter.drawLine(x1, y1, x2, y2);
    }
}

std::list<core::Line> CutterConstrainter::get3Lines(const core::BasicRegion& region) const
{
    auto linesList = region.getLines();
    std::vector<core::Line> lines(linesList.begin(), linesList.end());

    std::sort(lines.begin(), lines.end(), [this](const core::Line& line1, const core::Line& line2)
        { return line1.getDistance(startPoint.x, startPoint.y) < line2.getDistance(startPoint.x, startPoint.y); });

    linesList.clear();
    for (int i = 0; i < 3 && i < lines.size(); i++)
        linesList.push_back(lines[i]);

    return linesList;
}
