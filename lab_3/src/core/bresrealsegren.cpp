#include "bresrealsegren.hpp"

void core::BresRealSegRen::drawClampedSegment(QImage& image, Segment& segment)
{
    QRgb color = segment.color.rgba();
    startTiming();

    int x_start = std::round(segment.x1);
    int y_start = std::round(segment.y1);
    int x_end = std::round(segment.x2);
    int y_end = std::round(segment.y2);

    int deltaX = std::abs(x_end - x_start);
    int deltaY = std::abs(y_end - y_start);

    double error;
    double deltaError;

    if (deltaX == 0 && deltaY == 0)
    {
        image.setPixel(x_start, y_start, color);
    }
    else if (deltaX <= deltaY)
    {
        int x = x_start;

        deltaError = double(deltaX) / deltaY;
        error = deltaError;

        if (y_start <= y_end)
        {
            if (x_start <= x_end)
            {
                for (int y = y_start; y <= y_end; y++)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        x++;
                    }
                }
            }
            else
            {
                for (int y = y_start; y <= y_end; y++)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        x--;
                    }
                }
            }
        }
        else
        {
            if (x_start <= x_end)
            {
                for (int y = y_start; y >= y_end; y--)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        x++;
                    }
                }
            }
            else
            {
                for (int y = y_start; y >= y_end; y--)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        x--;
                    }
                }
            }
        }
    }
    else
    {
        int y = y_start;

        deltaError = (double)deltaY / deltaX;
        error = deltaError;

        if (x_start <= x_end)
        {
            if (y_start <= y_end)
            {
                for (int x = x_start; x <= x_end; x++)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        y++;
                    }
                }
            }
            else
            {
                for (int x = x_start; x <= x_end; x++)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        y--;
                    }
                }
            }
        }
        else
        {
            if (y_start <= y_end)
            {
                for (int x = x_start; x >= x_end; x--)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        y++;
                    }
                }
            }
            else
            {
                for (int x = x_start; x >= x_end; x--)
                {
                    image.setPixel(x, y, color);

                    error += deltaError;
                    if (error >= 1.0)
                    {
                        error -= 1.0;
                        y--;
                    }
                }
            }
        }
    }

    stopTiming(deltaX, deltaY);
}
