#include <QPixmap>
#include "stairscounter.hpp"

using namespace core;

double StairsCounter::countStairsLengthForSegment(SegmentRenderer* renderer, Segment segment)
{
    normalizeSegment(segment);

    int width = std::round(segment.x2) + 1;
    int height = std::abs(std::round(segment.y1) - std::round(segment.y2)) + 1;

    QPixmap pixmap(width, height);
    pixmap.fill();

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    renderer->drawSegment(image, segment);

    return countStairsLengthInImage(image);
}

int core::StairsCounter::countStairsAmountForSegment(SegmentRenderer* renderer, Segment segment)
{
    normalizeSegment(segment);

    int width = std::round(segment.x2) + 1;
    int height = std::abs(std::round(segment.y1) - std::round(segment.y2)) + 1;

    QPixmap pixmap(width, height);
    pixmap.fill();

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    renderer->drawSegment(image, segment);

    QTransform transform;
    transform.setMatrix(0, 1, 0, 1, 0, 0, 0, 0, 1);

    return std::min(countStairsAmountInImage(image), countStairsAmountInImage(image.transformed(transform)));
}

void core::StairsCounter::normalizeSegment(Segment& segment)
{
    if (segment.x1 > segment.x2)
    {
        std::swap(segment.x1, segment.x2);
        std::swap(segment.y1, segment.y2);
    }

    segment.x2 -= segment.x1;
    segment.x1 = 0.0;

    if (segment.y1 < segment.y2)
    {
        segment.y2 -= segment.y1;
        segment.y1 = 0.0;
    }
    else
    {
        segment.y1 -= segment.y2;
        segment.y1 = 0.0;
    }
}

double core::StairsCounter::countStairsLengthInImage(const QImage& image)
{
    int totalStairsLength = 0;
    int stairsCount = 0;

    for (int y = 0; y < image.height(); y++)
    {
        // analize row
        int x = 0;
        while (x < image.width() && QColor(image.pixel(x, y)) == Qt::white)
            x++;

        if (x < image.width())
        {
            stairsCount++;

            while (x < image.width() && QColor(image.pixel(x, y)) != Qt::white)
            {
                totalStairsLength++;
                x++;
            }
        }
    }

    return double(totalStairsLength) / stairsCount;
}

int core::StairsCounter::countStairsAmountInImage(const QImage& image)
{
    int stairsCount = 0;

    for (int y = 0; y < image.height(); y++)
    {
        // analize row
        int x = 0;
        while (x < image.width() && QColor(image.pixel(x, y)) == Qt::white)
            x++;

        if (x < image.width())
            stairsCount++;
    }

    return stairsCount;
}
