#pragma once

#include <QImage>
#include "segmentrenderer.hpp"

namespace core
{
    class StairsCounter
    {
    public:
        static double countStairsLengthForSegment(SegmentRenderer* renderer, Segment segment);
        static int countStairsAmountForSegment(SegmentRenderer* renderer, Segment segment);

    private:
        static void normalizeSegment(Segment& segment);
        static double countStairsLengthInImage(const QImage& image);
        static int countStairsAmountInImage(const QImage& image);
    };
}

