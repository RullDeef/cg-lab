#pragma once

#include <list>
#include <cmath>
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

            inline double distance(int x, int y) const
            {
                return hypot(this->x - x, this->y - y);
            }

            inline double distance(const Point& p) const
            {
                return hypot(x - p.x, y - p.y);
            }
        };

        struct Line
        {
            const Point* p1;
            const Point* p2;

            Line(const Point& p1, const Point& p2) : p1(&p1), p2(&p2) {}

            inline bool isHorizontal() const { return p1->y == p2->y; }

            inline operator Segment() const
            {
                Segment segment;
                segment.x1 = p1->x; segment.y1 = p1->y;
                segment.x2 = p2->x; segment.y2 = p2->y;
                return segment;
            }

            inline bool operator<(const Line& seg) const
            {
                return std::min(p1->y, p2->y) < std::min(seg.p1->y, seg.p2->y);
            }

            inline bool operator==(const Line& seg) const
            {
                return *p1 == *(seg.p1) && *p2 == *(seg.p2) || *p1 == *(seg.p2) && *p2 == *(seg.p1);
            }

            bool closeTo(int x, int y, double offset) const
            {
                double distance = 0;

                double v_x = p2->x - p1->x;
                double v_y = p2->y - p1->y;

                double w1_x = p1->x - x;
                double w1_y = p1->y - y;

                double w2_x = p2->x - x;
                double w2_y = p2->y - y;

                if (w1_x * v_x + w1_y * v_y >= 0)
                    distance = p1->distance(x, y);

                else if (w2_x * v_x + w2_y * v_y <= 0)
                    distance = p2->distance(x, y);

                else
                    distance = abs(w1_x * w2_y - w2_x * w1_y) / p1->distance(*p2);

                return distance < offset;
            }
        };

        std::list<Line>& getLines() { return lines; }
        const std::list<Line>& getLines() const { return lines; }
        std::list<Point>& getPoints() { return points; }
        const std::list<Point>& getPoints() const { return points; }

        BasicRegion clone() const
        {
            BasicRegion copy;
            copy.points = getPoints();
            copy.lines = getLines();

            for (auto &line : copy.lines)
            {
                for (const auto& point : copy.points)
                {
                    if (*(line.p1) == point) line.p1 = &point;
                    if (*(line.p2) == point) line.p2 = &point;
                }
            }

            return copy;
        }

        Point& appendPoint(int x, int y)
        {
            points.push_back(Point(x, y));
            return points.back();
        }

        void removePoint(const Point& point)
        {
            auto iter = std::find(points.begin(), points.end(), point);
            if (iter != points.end())
            {
                lines.erase(std::remove_if(lines.begin(), lines.end(),
                    [&point](const Line& line)
                        { return *(line.p1) == point || *(line.p2) == point; }),
                    lines.end());
                points.erase(iter);
            }
        }

        bool pointsConnected(const Point& p1, const Point& p2) const
        {
            Line connection(p1, p2);
            for (const auto& line : lines)
                if (line == connection)
                    return true;
            return false;
        }

        void connectPoints(const Point& p1, const Point& p2)
        {
            if (!pointsConnected(p1, p2))
                lines.push_back(Line(p1, p2));
        }

        void removeConnection(const Line& connection)
        {
            auto iter = std::find(lines.begin(), lines.end(), connection);
            lines.erase(iter);
        }

        void clear() { lines.clear();  points.clear(); }

        QRectF getBoundingBox() const;

    private:
        std::list<Point> points;
        std::list<Line> lines;
    };

    class AsyncFillStrategy
    {
    public:
        bool isFinished() const { return finished; }
        void beginFill() { finished = false; }
        void finish() { finished = true; }

    private:
        bool finished = false;
    };

    class RegionRenderer : public NamingStrategy, public TimingStrategy
    {
    public:
        RegionRenderer(const char* name) : NamingStrategy(name) {}

        virtual void fill(QImage& image, const BasicRegion& region, QColor color) = 0;
    };

    class AsyncRegionRenderer : public RegionRenderer, public AsyncFillStrategy
    {
    public:
        AsyncRegionRenderer(const char* name) : RegionRenderer(name) {}

        virtual void asyncFill(QImage& image, const BasicRegion& region, QColor color) = 0;
    };

    class BucFillRenderer : public NamingStrategy, public TimingStrategy
    {
    public:
        BucFillRenderer(const char* name) : NamingStrategy(name) {}

        virtual void fill(QImage& image, const QPoint& startPoint, QColor color) = 0;
    };

    class AsyncBucFillRenderer : public BucFillRenderer, public AsyncFillStrategy
    {
    public:
        AsyncBucFillRenderer(const char* name) : BucFillRenderer(name) {}

        virtual void asyncFill(QImage& image, const QPoint& startPoint, QColor color) = 0;
    };
}
