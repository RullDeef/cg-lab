#pragma once

#include <vector>
#include "primitiverenderer.hpp"

namespace core
{
    class BasicRegion : PrimitiveBase
    {
    public:
        struct Point
        {
            int x;
            int y;

            Point() = default;
            Point(int x, int y) : x(x), y(y) {}

            inline bool operator==(const Point& p) const
            {
                return x == p.x && y == p.y;
            }
        };

        struct Line
        {
            Point p1;
            Point p2;

            Line() = default;
            Line(const Point& p1, const Point& p2) : p1(p1), p2(p2) {}

            inline bool isHorizontal() const { return p1.y == p2.y; }

            inline operator Segment() const
            {
                Segment segment;
                segment.x1 = p1.x; segment.y1 = p1.y;
                segment.x2 = p2.x; segment.y2 = p2.y;
                return segment;
            }

            inline bool operator<(const Line& seg) const
            {
                return std::min(p1.y, p2.y) < std::min(seg.p1.y, seg.p2.y);
            }

            inline bool operator==(const Line& seg) const
            {
                return p1 == seg.p1 && p2 == seg.p2;
            }
        };

        std::vector<Line> getLines() const;
        std::vector<Point>& getPoints() { return points; }
        const std::vector<Point>& getPoints() const { return points; }
        Point& pointAt(size_t index) { return points[index]; }
        Point& appendPoint(int x, int y)
        {
            points.push_back(Point(x, y));
            return points.back();
        }

        void clear() { points.clear(); }

        QRectF getBoundingBox() const;

    private:
        std::vector<Point> points;
    };

    class RegionRenderer : public NamingStrategy, public TimingStrategy
    {
    public:
        RegionRenderer(const char* name) : NamingStrategy(name) {}

        virtual void fill(QImage& image, const BasicRegion& region, QColor color) = 0;
    };
}
