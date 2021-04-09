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
        int x_start = std::round(circle.x);
        int y_start = std::round(circle.y);
        int r_half = std::round(circle.r / std::sqrt(2.0));
        int dy = std::round(circle.r);
        int d = 0;

        for (int dx = 0; dx < r_half; dx++)
        {
            int d1 = d + dx + dx + 1;
            int d2 = d1 - dy - dy + 1;
            if (std::abs(d1) < std::abs(d2))
                d = d1;
            else
            {
                d = d2;
                dy--;
            }

            image.setPixel(x_start + dx, y_start + dy, color.rgba());
            image.setPixel(x_start - dx, y_start + dy, color.rgba());
            image.setPixel(x_start - dx, y_start - dy, color.rgba());
            image.setPixel(x_start + dx, y_start - dy, color.rgba());
            image.setPixel(x_start + dy, y_start + dx, color.rgba());
            image.setPixel(x_start - dy, y_start + dx, color.rgba());
            image.setPixel(x_start - dy, y_start - dx, color.rgba());
            image.setPixel(x_start + dy, y_start - dx, color.rgba());
        }
    }

    endTiming();
}
