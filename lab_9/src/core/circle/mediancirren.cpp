#include "mediancirren.hpp"
#include <QtMath>

void core::MedianCircleRenderer::draw(QImage& image, const Circle& circle, QColor color)
{
    constexpr auto tau = qDegreesToRadians(360.0);

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
        image.setPixel(x0 + x, y0 + y, color.rgba());
        image.setPixel(x0 - x, y0 + y, color.rgba());
        image.setPixel(x0 - x, y0 - y, color.rgba());
        image.setPixel(x0 + x, y0 - y, color.rgba());

        int f = std::round(1.25 - circle.r);

        while (y >= x)
        {
            if (f <= 0)
                f += 2 * ++x + 1;
            else
                f += 2 * ++x + 1 - 2 * --y;

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
