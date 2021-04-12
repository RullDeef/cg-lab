#include "brescirren.hpp"

void core::BresenhemCircleRenderer::draw(QImage& image, const Circle& circle, QColor color)
{
    beginTiming();

    if (circle.r == 0.0)
    {
        int x = std::round(circle.x);
        int y = std::round(circle.y);

        image.setPixel(x, y, color.rgba());
    }
    else
    {
        int x0 = std::round(circle.x);
        int y0 = std::round(circle.y);
        int R = std::round(circle.r);

        int x = 0;
        int y = R;
        int d = 2 * (1 - R);

        image.setPixel(x0, y0 + R, color.rgba());
        image.setPixel(x0, y0 - R, color.rgba());
        image.setPixel(x0 + R, y0, color.rgba());
        image.setPixel(x0 - R, y0, color.rgba());

        while (y >= x)
        {
            if (d < 0)
            {
                if (2 * (d + y) - 1 < 0) // horizontal move
                {
                    x++;
                    d += 2 * x + 1;
                }
                else // diagonal move
                {
                    x++;
                    y--;
                    d += 2 * (x - y + 1);
                }
            }
            else if (d > 0)
            {
                if (2 * (d - x) - 1 < 0) // diagonal move
                {
                    x++;
                    y--;
                    d += 2 * (x - y + 1);
                }
                else // vertical move
                {
                    y--;
                    d += -2 * y + 1;
                }
            }
            else // diagonal move
            {
                x++;
                y--;
                d += 2 * (x - y + 1);
            }

            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 - y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
            image.setPixel(x0 + y, y0 + x, color.rgba());
            image.setPixel(x0 - y, y0 + x, color.rgba());
            image.setPixel(x0 - y, y0 - x, color.rgba());
            image.setPixel(x0 + y, y0 - x, color.rgba());
        }
    }

    endTiming();
}
