#include "bressmoothsegren.hpp"

void core::BresSmoothSegmentRenderer::draw(QImage& image, const Segment& segment, QColor color)
{
    Segment seg = segment;

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

    beginTiming();

    if (seg.x2 < seg.x1) // swap end points
    {
        std::swap(seg.x1, seg.x2);
        std::swap(seg.y1, seg.y2);
    }

    int x = seg.x1;
    int y = seg.y1;

    int deltaX = abs(seg.x2 - seg.x1) + 1;
    int deltaY = abs(seg.y2 - seg.y1) + 1;

    int yDir = seg.y1 < seg.y2 ? 1 : -1;

    float S_i = 0.0;
    float m;

    if (deltaX < deltaY)
    {
        m = std::abs((float)deltaX / deltaY);

        while (yDir > 0 && y < seg.y2 + yDir
            || yDir < 0 && y > seg.y2 + yDir)
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

        while (x < seg.x2 + 1)
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

    endTiming();
}
