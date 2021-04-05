#include "bressmoothsegren.hpp"

void core::BresSmoothSegRen::drawClampedSegment(QImage& image, Segment& segment)
{
    QColor color = segment.color;

    const auto plot = [&image, &color](int x, int y, float alpha)
    {
        QColor dst = image.pixel(x, y);
        QColor clr(
            dst.red() * (1 - alpha) + color.red() * alpha,
            dst.green() * (1 - alpha) + color.green() * alpha,
            dst.blue() * (1 - alpha) + color.blue() * alpha,
            255
        );
        image.setPixel(x, y, clr.rgba());
    };

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

    float S_i = 0.0;
    float m;

    if (deltaX < deltaY)
    {
        m = std::abs((float)deltaX / deltaY);

        while (yDir > 0 && y < segment.y2 + yDir
            || yDir < 0 && y > segment.y2 + yDir)
        {
            S_i += m;
            if (S_i >= 1.0)
            {
                S_i -= 1.0;
                plot(x, y, 1.0 - S_i);
                x++;
            }

            plot(x, y, S_i);
            y += yDir;
        }
    }
    else
    {
        m = std::abs((float)deltaY / deltaX);

        while (x < segment.x2 + 1)
        {
            S_i += m;
            if (S_i >= 1.0)
            {
                S_i -= 1.0;
                plot(x, y, 1.0 - S_i);
                y += yDir;
            }

            plot(x, y, S_i);
            x++;
        }
    }

    stopTiming(deltaX, deltaY);
}
