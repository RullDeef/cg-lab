#include "canonellren.hpp"
#include <QtMath>

void core::CanonicEllipseRenderer::draw(QImage& image, const Ellipse& ellipse, QColor color)
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

        double A2 = ellipse.a * ellipse.a;
        double B2 = ellipse.b * ellipse.b;

        for (int x = -A; x <= A; x++)
        {
            int y = std::round(ellipse.b * std::sqrt(1 - double(x) * x / (A * A)));
            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
        }

        for (int y = -B; y <= B; y++)
        {
            int x = std::round(ellipse.a * std::sqrt(1 - double(y) * y / (B * B)));
            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
        }
    }

    endTiming();
}
