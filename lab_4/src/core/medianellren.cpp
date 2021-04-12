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

        int A2 = A * A;
        int B2 = B * B;
        int A22 = 2 * A2;
        int B22 = 2 * B2;

        int x = 0;
        int y = B;
        int f = std::round(B2 - A2 * B + 0.25 * A2);

        image.setPixel(x0, y0 + y, color.rgba());
        image.setPixel(x0, y0 - y, color.rgba());

        while (y * A2 > x * B2)
        {
            if (f <= 0)
                f += B22 * ++x + B2;
            else
                f += B22 * ++x - A22 * --y + B2;

            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 - y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
        }

        x = A;
        y = 0;
        f = std::round(A2 - B2 * A + 0.25 * B2);

        image.setPixel(x0 + x, y0, color.rgba());
        image.setPixel(x0 - x, y0, color.rgba());

        while (x * B2 > y * A2)
        {
            if (f <= 0)
                f += A22 * ++y + A2;
            else
                f += A22 * ++y - B22 * --x + A2;

            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 - y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
        }
    }

    endTiming();
}
