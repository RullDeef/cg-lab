#pragma once

#include <QPainter>
#include <chrono>

namespace core
{
    struct PrimitiveBase
    {
        virtual QRectF getBoundingBox() const = 0;
    };

    struct Segment : public PrimitiveBase
    {
        double x1;
        double y1;
        double x2;
        double y2;

        QRectF getBoundingBox() const override
        {
            return QRectF(std::min(x1, x2), std::min(y1, y2), std::abs(x2 - x1), std::abs(y2 - y1));
        }
    };

    struct Circle : public PrimitiveBase
    {
        double x;
        double y;
        double r;

        QRectF getBoundingBox() const override
        {
            return QRectF(x - r, y - r, 2 * r, 2 * r);
        }
    };

    struct Ellipse : public PrimitiveBase
    {
        double x;
        double y;
        double a;
        double b;

        QRectF getBoundingBox() const override
        {
            return QRectF(x - a, y - b, 2 * a, 2 * b);
        }
    };

    class NamingStrategy
    {
    public:
        NamingStrategy(const char* name) : name(name) {}
        const char* getName() { return name; }

    private:
        const char* name;
    };

    class TimingStrategy
    {
        using durr_t = std::chrono::nanoseconds;
        using time_point_t = std::chrono::high_resolution_clock::time_point;

    public:
        durr_t getDuration() { return std::chrono::duration_cast<durr_t>(endTime - startTime); }

    protected:
        void beginTiming() { startTime = std::chrono::high_resolution_clock::now(); }
        void endTiming() { endTime = std::chrono::high_resolution_clock::now(); }

    private:
        time_point_t startTime;
        time_point_t endTime;
    };

    template<typename PrimitiveType, typename std::enable_if<std::is_base_of<PrimitiveBase, PrimitiveType>::value>::type* = nullptr>
    class PrimitiveRenderer : public NamingStrategy, public TimingStrategy
    {
    public:
        PrimitiveRenderer(const char* name) : NamingStrategy(name) {}

        virtual void draw(QImage& image, const PrimitiveType& primitive, QColor color) = 0;
    };
}
