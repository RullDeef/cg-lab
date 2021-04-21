#include "medianellren.hpp"
#include <QtMath>

void core::MedianEllipseRenderer::draw(QImage& image, const Ellipse& ellipse, QColor color)
{
    constexpr auto tau = qDegreesToRadians(360.0);

    beginTiming();

    if (ellipse.a == 0.0 && ellipse.b == 0.0)
    {
        int x = std::round(ellipse.x);
        int y = std::round(ellipse.y);

        image.setPixel(x, y, color.rgba());
    }
    else
    {
        int x0 = std::round(ellipse.x);
        int y0 = std::round(ellipse.y);
        int A = std::round(ellipse.a);
        int B = std::round(ellipse.b);

        int x = 0;
        int y = B;
        image.setPixel(x0 + x, y0 + y, color.rgba());
        image.setPixel(x0 - x, y0 + y, color.rgba());
        image.setPixel(x0 - x, y0 - y, color.rgba());
        image.setPixel(x0 + x, y0 - y, color.rgba());

        int f = std::round(B * B - A * A * B + 0.25 * A * A);

        while (y * A * A > x * B * B)
        {
            if (f <= 0)
                f += 2 * B * B * ++x + B * B;
            else
                f += 2 * B * B * ++x - 2 * A * A * --y + B * B;

            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 - y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
            image.setPixel(x0 + y, y0 + x, color.rgba());
            image.setPixel(x0 - y, y0 + x, color.rgba());
            image.setPixel(x0 - y, y0 - x, color.rgba());
            image.setPixel(x0 + y, y0 - x, color.rgba());
        }

        f = std::round(B * B * (x + 0.5) * (x + 0.5) + A * A * (y - 1) * (y - 1) - A * A * B * B);

        while (y > 0)
        {
            if (f >= 0)
                f += -2 * A * A * --y + A * A;
            else
                f += 2 * B * B * ++x - 2 * A * A * --y + A * A;

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
