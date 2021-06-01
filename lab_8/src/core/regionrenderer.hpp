#pragma once

#include <list>
#include <cmath>
#include "primitiverenderer.hpp"

namespace core
{
    struct Point
    {
        int x;
        int y;

        Point() = default;
        Point(int x, int y) : x(x), y(y) {}

        bool operator==(const Point& p) const;
        bool operator!=(const Point& p) const;

        double distance(int x, int y) const;
        double distance(const Point& p) const;
    };

    struct Vector
    {
        double dx, dy;

        explicit Vector(double dx = 0.0, double dy = 0.0);
        explicit Vector(const Point& p1, const Point& p2);
        explicit Vector(const Segment& seg);

        inline Vector rotate90cw() const { return Vector(dy, -dx); }
        inline Vector rotate90ccw() const { return Vector(-dy, dx); }

        inline double dot(const Vector& v) const { return dx * v.dx + dy * v.dy; }

        double getLength() const;
        void setLength(double len);

        operator Segment() const;
    };

    struct Line
    {
        const Point* p1;
        const Point* p2;

        Line(const Point& p1, const Point& p2) : p1(&p1), p2(&p2) {}

        inline bool isHorizontal() const { return p1->y == p2->y; }
        inline bool isVertical() const { return p1->x == p2->x; }
        inline bool contains(const Point& p) const { return &p == p1 || &p == p2; }

        inline void reverse() { std::swap(p1, p2); }
        inline Line reversed() const { return Line(*p2, *p1); }

        inline double length() const { return p1->distance(*p2); }

        operator Segment() const;

        bool operator==(const Line& line) const;
        bool operator!=(const Line& line) const;

        bool closeTo(int x, int y, double offset) const;
        double cross(const Line& line) const;
    };

    class BasicRegion : PrimitiveBase
    {
    public:
        inline std::list<Line>& getLines() { return lines; }
        inline const std::list<Line>& getLines() const { return lines; }
        inline std::list<Point>& getPoints() { return points; }
        inline const std::list<Point>& getPoints() const { return points; }

        BasicRegion clone() const;
        Point& appendPoint(int x, int y);
        
        void connectPoints(const Point& p1, const Point& p2);
        bool pointsConnected(const Point& p1, const Point& p2) const;

        void removePoint(const Point& point);
        void removeConnection(const Line& connection);

        void clear();

        QRectF getBoundingBox() const;

        bool isConvex() const;

        std::list<std::list<Line>> getContours() const;

        std::list<Line> getLinesConnected(const Point& p) const;

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
