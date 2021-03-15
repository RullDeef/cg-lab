#include "bresintsegren.hpp"

void core::BresIntSegRen::drawClampedSegment(QImage& image, Segment& segment)
{
    QRgb color = segment.color.rgba();
    startTiming();

    if (segment.x2 < segment.x1) // swap end points
    {
        std::swap(segment.x1, segment.x2);
        std::swap(segment.y1, segment.y2);
    }

    int x = segment.x1;
    int y = segment.y1;

    int deltaX = abs(segment.x2 - segment.x1) + 1;
    int deltaY = abs(segment.y2 - segment.y1) + 1;

    int yDir = segment.y1 < segment.y2 ? 1 : -1;

    int error = 0;
    int deltaError;

    if (deltaX < deltaY)
    {
        deltaError = deltaX;

        while (yDir > 0 && y < segment.y2 + yDir
            || yDir < 0 && y > segment.y2 + yDir)
        {
            image.setPixel(x, y, color);

            error += deltaError;
            if (error >= deltaY)
            {
                error -= deltaY;
                x++;
            }

            y += yDir;
        }
    }
    else
    {
        deltaError = deltaY;

        while (x < segment.x2 + 1)
        {
            image.setPixel(x, y, color);

            error += deltaError;
            if (error >= deltaX)
            {
                error -= deltaX;
                y += yDir;
            }

            x++;
        }
    }

    stopTiming(deltaX, deltaY);
}
