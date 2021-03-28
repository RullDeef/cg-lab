#pragma once

#include <chrono>
#include <QImage>

enum class AlgorithmType
{
    Parameterised = 0,
    Library = 1
};

struct Ellipse
{
    double x, y;
    double width;
    double height;

    QColor color;
};

class EllipseRenderer
{
public:
    using durr_t = std::chrono::nanoseconds;
    using time_point_t = std::chrono::high_resolution_clock::time_point;

    const static size_t defaultQueueSize = 100;

    EllipseRenderer(const char* name, size_t queueSize = defaultQueueSize) : name(name),
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

    virtual void drawEllipse(QImage& image, const Ellipse& ellipse) = 0;

protected:
    void startTiming();
    void stopTiming(double dx, double dy);

private:
    const char* name;
    time_point_t startTime;

    size_t queueSize;
    std::list<double> drawTimes;
};

