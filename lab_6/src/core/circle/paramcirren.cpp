#include "paramcirren.hpp"
#include <QtMath>

void core::ParametricCircleRenderer::draw(QImage& image, const Circle& circle, QColor color)
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
        double dt = 1 / circle.r;
        for (double t = 0.0; t < tau; t += dt)
        {
            int x = std::round(circle.x + circle.r * std::cos(t));
            int y = std::round(circle.y + circle.r * std::sin(t));

            image.setPixel(x, y, color.rgba());
        }
    }

    endTiming();
}
