#include "bresrealsegren.hpp"

void core::BresRealSegRen::drawClampedSegment(QImage& image, Segment& segment)
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

    double error = 0.0;
    double deltaError;

    if (deltaX < deltaY)
    {
        deltaError = static_cast<double>(deltaX) / deltaY;

        while (yDir > 0 && y < segment.y2 + yDir
            || yDir < 0 && y > segment.y2 + yDir)
        {
            image.setPixel(x, y, color);

            error += deltaError;
            if (error >= 1.0)
            {
                error -= 1.0;
                x++;
            }

            y += yDir;
        }
    }
    else
    {
        deltaError = static_cast<double>(deltaY) / deltaX;

        while (x < segment.x2 + 1)
        {
            image.setPixel(x, y, color);

            error += deltaError;
            if (error >= 1.0)
            {
                error -= 1.0;
                y += yDir;
            }

            x++;
        }
    }

    stopTiming();
}
