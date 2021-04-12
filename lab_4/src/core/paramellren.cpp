#include "paramellren.hpp"
#include <QtMath>

void core::ParametricEllipseRenderer::draw(QImage& image, const Ellipse& ellipse, QColor color)
{
    constexpr auto tau = qDegreesToRadians(360.0);

    beginTiming();

    if (ellipse.a == 0.0 && ellipse.b == 0)
    {
        int x = std::round(ellipse.x);
        int y = std::round(ellipse.y);

        image.setPixel(x, y, color.rgba());
    }
    else
    {
        double dt = 1 / std::max(ellipse.a, ellipse.b);
        for (double t = 0.0; t < tau; t += dt)
        {
            int x = std::round(ellipse.x + ellipse.a * std::cos(t));
            int y = std::round(ellipse.y + ellipse.b * std::sin(t));

            image.setPixel(x, y, color.rgba());
        }
    }

    endTiming();
}
