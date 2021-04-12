#include "bresellren.hpp"

void core::BresenhemEllipseRenderer::draw(QImage& image, const Ellipse& ellipse, QColor color)
{
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
        int A2pB2 = A2 + B2;

        int x = 0;
        int y = B;
        int d = 2 * B * (B - A2);

        image.setPixel(x0, y0 + B, color.rgba());
        image.setPixel(x0, y0 - B, color.rgba());
        image.setPixel(x0 + A, y0, color.rgba());
        image.setPixel(x0 - A, y0, color.rgba());

        while (y >= 0)
        {
            if (d < 0)
            {
                if (2 * d + A22 * y - A2 < 0) // horizontal move
                {
                    x++;
                    d += B22 * x + B2;
                }
                else // diagonal move
                {
                    x++;
                    y--;
                    d += B22 * x - A22 * y + A2pB2;
                }
            }
            else if (d > 0)
            {
                if (2 * d - B22 * x - B2 < 0) // diagonal move
                {
                    x++;
                    y--;
                    d += B22 * x - A22 * y + A2pB2;
                }
                else // vertical move
                {
                    y--;
                    d += -A22 * y + A2;
                }
            }
            else // diagonal move
            {
                x++;
                y--;
                d += B22 * x - A22 * y + A2pB2;
            }

            image.setPixel(x0 + x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 + y, color.rgba());
            image.setPixel(x0 - x, y0 - y, color.rgba());
            image.setPixel(x0 + x, y0 - y, color.rgba());
        }
    }

    endTiming();
}
