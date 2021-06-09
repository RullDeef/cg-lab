#include "canoncirren.hpp"
#include <QtMath>

void core::CanonicCircleRenderer::draw(QImage& image, const Circle& circle, QColor color)
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
        int R2 = std::round(circle.r / std::sqrt(2));

        for (int x = -R2; x <= R2; x++)
        {
            int y = std::round(std::sqrt(R * R - x * x));
            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
        }

        for (int y = -R2; y <= R2; y++)
        {
            int x = std::round(std::sqrt(R * R - y * y));
            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
        }
    }

    endTiming();
}
