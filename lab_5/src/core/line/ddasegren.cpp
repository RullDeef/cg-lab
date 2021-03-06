#include <cmath>
#include "ddasegren.hpp"

void core::DDASegmentRenderer::draw(QImage& image, const Segment& segment, QColor color)
{
    const auto round = [](double a) -> int { return a <= std::floor(a) + 0.5 ? std::floor(a) : std::ceil(a); };

    beginTiming();

    int x_start = std::round(segment.x1);
    int y_start = std::round(segment.y1);
    int x_end = std::round(segment.x2);
    int y_end = std::round(segment.y2);

    unsigned int deltaX = std::abs(x_end - x_start);
    unsigned int deltaY = std::abs(y_end - y_start);

    if (deltaX == 0 && deltaY == 0)
    {
        image.setPixel(x_start, y_start, color.rgba());
    }
    else if (deltaX <= deltaY)
    {
        double x = x_start;
        double dx = double(x_end - x_start) / (y_end - y_start);

        if (y_start <= y_end)
        {
            for (int y = y_start; y <= y_end; y++, x += dx)
                image.setPixel(round(x), y, color.rgba());
        }
        else
        {
            for (int y = y_start; y >= y_end; y--, x -= dx)
                image.setPixel(round(x), y, color.rgba());
        }
    }
    else
    {
        double y = y_start;
        double dy = double(y_end - y_start) / (x_end - x_start);

        if (x_start <= x_end)
        {
            for (int x = x_start; x <= x_end; x++, y += dy)
                image.setPixel(x, round(y), color.rgba());
        }
        else
        {
            for (int x = x_start; x >= x_end; x--, y -= dy)
                image.setPixel(x, round(y), color.rgba());
        }
    }

    endTiming();
}
