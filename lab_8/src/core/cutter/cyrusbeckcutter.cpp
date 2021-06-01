#include "cyrusbeckcutter.hpp"

constexpr auto EPS = 1e-5;

void core::CyrusBeckCutter::cut(std::list<Segment>& lines, const BasicRegion& cutter)
{
    std::list<Segment> initialLines = lines;
    lines.clear();

    for (auto& line : initialLines)
    {
        double tStart = 0.0;
        double tEnd = 1.0;

        Vector D(line);

        for (auto& cutterLine : cutter.getLines())
        {
            Vector n = Vector(cutterLine).rotate90ccw();
            Vector W(*cutterLine.p1, Point(line.x1, line.y1));

            double nD = n.dot(D);
            double nW = n.dot(W);

            if (nD > EPS)
            {
                if (nW <= 0.0)
                {
                    double t = -nW / nD;
                    tStart = std::max(t, tStart);
                    if (t > 1.0)
                        break;
                }
            }
            else if (nD < -EPS)
            {
                if (nW < 0.0)
                {
                    tStart = 2.0;
                    break;
                }
                else
                {
                    double t = -nW / nD;
                    tEnd = std::min(t, tEnd);
                    if (t < 0.0)
                        break;
                }
            }
        }

        if (tStart < tEnd)
        {
            line.x2 = line.x1 + tEnd * D.dx;
            line.y2 = line.y1 + tEnd * D.dy;
            line.x1 = line.x1 + tStart * D.dx;
            line.y1 = line.y1 + tStart * D.dy;

            lines.push_back(line);
        }
    }
}
