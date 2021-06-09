#include "simplecutter.hpp"


void core::SimpleCutter::cut(std::list<Segment>& lines, const Rect& cutter)
{
    std::list<LinePatch> patches(lines.begin(), lines.end());
    lines.clear();

    beginTiming();

    int cLeft = cutter.left;
    int cTop = cutter.top;
    int cRight = cutter.left + cutter.width;
    int cBottom = cutter.top + cutter.height;

    for (auto& line : patches)
    {
        // установить битовые поля отрезка
        int T1 = 0, T2 = 0;

        if (line.p1x < cLeft)      T1 |= BIT_LEFT;
        if (line.p1x > cRight)     T1 |= BIT_RIGHT;
        if (line.p1y < cTop)       T1 |= BIT_TOP;
        if (line.p1y > cBottom)    T1 |= BIT_BOTTOM;

        if (line.p2x < cLeft)      T2 |= BIT_LEFT;
        if (line.p2x > cRight)     T2 |= BIT_RIGHT;
        if (line.p2y < cTop)       T2 |= BIT_TOP;
        if (line.p2y > cBottom)    T2 |= BIT_BOTTOM;

        // тривиально видимый отрезок
        if ((T1 | T2) == 0)
        {
            lines.push_back(line);
            continue;
        }

        // тривиально невидимый отрезок
        if ((T1 & T2) != 0)
            continue;

        // если отрезок горизонтален
        if (line.p1y == line.p2y)
        {
            if (line.p1x < line.p2x)
            {
                line.p1x = std::max(line.p1x, cLeft);
                line.p2x = std::min(line.p2x, cRight);

                if (line.p1x > line.p2x)
                    continue; // отрезок невидим
            }
            else
            {
                line.p2x = std::max(line.p2x, cLeft);
                line.p1x = std::min(line.p1x, cRight);

                if (line.p1x < line.p2x)
                    continue; // отрезок невидим
            }

            lines.push_back(line);
        }
        // иначе если отрезок вертикален
        else if (line.p1x == line.p2x)
        {
            if (line.p1y < line.p2y)
            {
                line.p1y = std::max(line.p1y, cTop);
                line.p2y = std::min(line.p2y, cBottom);

                if (line.p1y > line.p2y)
                    continue; // отрезок невидим
            }
            else
            {
                line.p2y = std::max(line.p2y, cTop);
                line.p1y = std::min(line.p1y, cBottom);

                if (line.p1y < line.p2y)
                    continue; // отрезок невидим
            }

            lines.push_back(line);
        }
        // отрезок общего положения
        else
        {
            double m = double(line.p2y - line.p1y) / (line.p2x - line.p1x);

            while (!((T1 | T2) == 0 || (T1 & T2) != 0))
            {
                if (T1 != 0)
                {
                    if (line.p1x < cLeft)
                    {
                        line.p1y += m * (cLeft - line.p1x);
                        line.p1x = cLeft;
                        T1 &= ~BIT_LEFT;
                    }
                    else if (line.p1x > cRight)
                    {
                        line.p1y += m * (cRight - line.p1x);
                        line.p1x = cRight;
                        T1 &= ~BIT_RIGHT;
                    }
                    else if (line.p1y < cTop)
                    {
                        line.p1x += (1 / m) * (cTop - line.p1y);
                        line.p1y = cTop;
                        T1 &= ~BIT_TOP;
                    }
                    else if (line.p1y > cBottom)
                    {
                        line.p1x += (1 / m) * (cBottom - line.p1y);
                        line.p1y = cBottom;
                        T1 &= ~BIT_BOTTOM;
                    }
                }

                if (T2 != 0)
                {
                    if (line.p2x < cLeft)
                    {
                        line.p2y += m * (cLeft - line.p2x);
                        line.p2x = cLeft;
                        T2 &= ~BIT_LEFT;
                    }
                    else if (line.p2x > cRight)
                    {
                        line.p2y += m * (cRight - line.p2x);
                        line.p2x = cRight;
                        T2 &= ~BIT_RIGHT;
                    }
                    else if (line.p2y < cTop)
                    {
                        line.p2x += (1 / m) * (cTop - line.p2y);
                        line.p2y = cTop;
                        T2 &= ~BIT_TOP;
                    }
                    else if (line.p2y > cBottom)
                    {
                        line.p2x += (1 / m) * (cBottom - line.p2y);
                        line.p2y = cBottom;
                        T2 &= ~BIT_BOTTOM;
                    }
                }

                // if (line.p1x < cLeft)      T1 |= BIT_LEFT;
                // if (line.p1x > cRight)     T1 |= BIT_RIGHT;
                // if (line.p1y < cTop)       T1 |= BIT_TOP;
                // if (line.p1y > cBottom)    T1 |= BIT_BOTTOM;

                // if (line.p2x < cLeft)      T2 |= BIT_LEFT;
                // if (line.p2x > cRight)     T2 |= BIT_RIGHT;
                // if (line.p2y < cTop)       T2 |= BIT_TOP;
                // if (line.p2y > cBottom)    T2 |= BIT_BOTTOM;
            }

            if ((T1 & T2) != 0)
                 continue;
        }

        lines.push_back(line);
    }

    endTiming();
}
