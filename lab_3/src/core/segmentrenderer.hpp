#pragma once

#include <QString>
#include <QImage>
#include <list>
#include <algorithm>
#include <chrono>

namespace core
{
    enum class AlgorithmType
    {
        DDA = 0,
        BresenhemReal = 1,
        BresenhemInt = 2,
        BresenhemExt = 3,
        Library = 4
    };

    struct Segment
    {
        double x1, y1, x2, y2;
        QColor color;
    };

    class SegmentRenderer // interface
    {
    public:
        using durr_t = std::chrono::microseconds;
        using time_point_t = std::chrono::high_resolution_clock::time_point;

        const static size_t defaultQueueSize = 10;

        SegmentRenderer(const char* name, size_t queueSize = defaultQueueSize) : name(name),
            queueSize(queueSize), startTime(std::chrono::high_resolution_clock::now()) {}

        const char* getName() const { return name; }
        void drawSegment(QImage& image, Segment segment);
        
        inline durr_t getLastDrawTime() const
        {
            return drawTimes.size() == 0 ? durr_t(0) : drawTimes.front();
        }
        
        inline durr_t getMeanDrawTime() const
        {
            return drawTimes.size() == 0 ? durr_t(0) : std::accumulate(drawTimes.begin(),
                drawTimes.end(), durr_t(0), std::plus()) / drawTimes.size();
        }

    protected:
        void startTiming();
        void stopTiming();

        virtual void drawClampedSegment(QImage& image, Segment& segment) = 0;

    private:
        const char* name;
        time_point_t startTime;

        size_t queueSize;
        std::list<durr_t> drawTimes;
    };
}
