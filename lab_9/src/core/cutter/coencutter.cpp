#include "coencutter.hpp"

using namespace core;

static int cutterBounds[4]; // { left, right, bottom, top }

static int pointCode(int x, int y)
{
    int code = 0;

    if (x < cutterBounds[0]) code |= BIT_LEFT;
    if (x > cutterBounds[1]) code |= BIT_RIGHT;
    if (y > cutterBounds[2]) code |= BIT_BOTTOM;
    if (y < cutterBounds[3]) code |= BIT_TOP;

    return code;
}

static bool cutHorSeg(LinePatch& patch)
{
    if (patch.p1x < patch.p2x)
    {
        patch.p1x = std::max(patch.p1x, cutterBounds[0]);
        patch.p2x = std::min(patch.p2x, cutterBounds[1]);

        if (patch.p1x > patch.p2x)
            return false; // ������� �������
    }
    else
    {
        patch.p2x = std::max(patch.p2x, cutterBounds[0]);
        patch.p1x = std::min(patch.p1x, cutterBounds[1]);

        if (patch.p1x < patch.p2x)
            return false; // ������� �������
    }

    return true;
}

static bool cutVerSeg(LinePatch& p)
{
    if (p.p1y < p.p2y)
    {
        p.p1y = std::max(p.p1y, cutterBounds[3]);
        p.p2y = std::min(p.p2y, cutterBounds[2]);

        if (p.p1y > p.p2y)
            return false; // ������� �������
    }
    else
    {
        p.p2y = std::max(p.p2y, cutterBounds[3]);
        p.p1y = std::min(p.p1y, cutterBounds[2]);

        if (p.p1y < p.p2y)
            return false; // ������� �������
    }

    return true;
}

void core::CoenCutter::cut(std::list<Segment>& lines, const Rect& cutter)
{
    std::list<LinePatch> patches(lines.begin(), lines.end());
    lines.clear();

    cutterBounds[0] = cutter.left;
    cutterBounds[1] = cutter.left + cutter.width;
    cutterBounds[2] = cutter.top + cutter.height;
    cutterBounds[3] = cutter.top;

    for (auto& p : patches)
    {
        // ���������� ������� ���� ��������
        int T1 = pointCode(p.p1x, p.p1y);
        int T2 = pointCode(p.p2x, p.p2y);

        // ����������� ���������
        if ((T1 | T2) == 0)
        {
            lines.push_back(p);
            continue;
        }

        // ����������� �����������
        if ((T1 & T2) != 0)
            continue;

        if (p.p1y == p.p2y) // ���� ������� ������������
        {
            if (!cutHorSeg(p))
                continue;
            lines.push_back(p);
        }
        else if (p.p1x == p.p2x) // ����� ���� ������� ����������
        {
            if (!cutVerSeg(p))
                continue;
            lines.push_back(p);
        }
        else // ������� ������ ���������
        {
            double  m = double(p.p2y - p.p1y) / (p.p2x - p.p1x);
            double _m = double(p.p2x - p.p1x) / (p.p2y - p.p1y);;

            for (int i = 0; i < 4; i++)
            {
                // ����������� ��������� ��� ����������� �����������
                if ((T1 | T2) == 0 || (T1 & T2) != 0)
                    break;

                // ���� ������� ����� ������ - ������ ����� �������
                if ((T1 & (1 << i)) == 0)
                {
                    std::swap(p.p1x, p.p2x);
                    std::swap(p.p1y, p.p2y);
                    std::swap(T1, T2);
                }

                if (i < 2) // ��������� ����� � ������ ��������� ����������
                {
                    int y = p.p1y + m * (cutterBounds[i] - p.p1x);

                    // �������� ������������ �����������
                    if ((p.p1y <= y && y <= p.p2y) || (p.p2y <= y && y <= p.p1y))
                    {
                        p.p1y = y;
                        p.p1x = cutterBounds[i];
                        T1 = pointCode(p.p1x, p.p1y);
                    }
                }
                else // ��������� ������� � ������ ��������� ����������
                {
                    int x = p.p1x + _m * (cutterBounds[i] - p.p1y);

                    // �������� ������������ �����������
                    if ((p.p1x <= x && x <= p.p2x) || (p.p2x <= x && x <= p.p1x))
                    {
                        p.p1x = x;
                        p.p1y = cutterBounds[i];
                        T1 = pointCode(p.p1x, p.p1y);
                    }
                }
            }

            // ���������
            if ((T1 | T2) == 0)
                lines.push_back(p);
        }
    }
}
