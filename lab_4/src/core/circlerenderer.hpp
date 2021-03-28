#pragma once

#include <chrono>
#include <QImage>

struct Circle
{
    double x, y;
    double radius;

    QColor color;
};

class CircleRenderer
{
public:
    using durr_t = std::chrono::nanoseconds;
    using time_point_t = std::chrono::high_resolution_clock::time_point;

    const static size_t defaultQueueSize = 100;

    CircleRenderer(const char* name, size_t queueSize = defaultQueueSize) : name(name),
        queueSize(queueSize), startTime(std::chrono::high_resolution_clock::now()) {}

    const char* getName() const { return name; }

    inline double getLastDrawTime() const
    {
        return drawTimes.size() == 0 ? 0.0 : drawTimes.front();
    }

    inline double getMeanDrawTime() const
    {
        return drawTimes.size() == 0 ? 0.0 : std::accumulate(drawTimes.begin(),
            drawTimes.end(), 0.0, std::plus()) / drawTimes.size();
    }

    virtual void drawCircle(QImage& image, const Circle& circle) = 0;

protected:
    void startTiming();
    void stopTiming(double r);

private:
    const char* name;
    time_point_t startTime;

    size_t queueSize;
    std::list<double> drawTimes;
};

