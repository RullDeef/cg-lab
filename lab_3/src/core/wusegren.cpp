#include "wusegren.hpp"

void core::WuSegRen::drawClampedSegment(QImage& image, Segment& segment)
{
    QColor color = segment.color;

    auto ipart = [](float x) -> int { return int(std::floor(x)); };
    auto round = [](float x) -> float { return std::round(x); };
    auto fpart = [](float x) -> float { return x - std::floor(x); };
    auto rfpart = [](float x) -> float { return 1 + std::floor(x) - x; };

    auto plot = [&](int x, int y, float alpha)
    {
        QColor dst = image.pixel(x, y);
        QColor clr(
            dst.red() * (1 - alpha) + color.red() * alpha,
            dst.green() * (1 - alpha) + color.green() * alpha,
            dst.blue() * (1 - alpha) + color.blue() * alpha,
            std::min(255, dst.alpha() + int(255 * alpha))
        );
        image.setPixel(x, y, clr.rgba());
    };

    startTiming();

    const bool step = abs(segment.y2 - segment.y1) > abs(segment.x2 - segment.x1);
    if (step)
    {
        std::swap(segment.x1, segment.y1);
        std::swap(segment.x2, segment.y2);
    }
    if (segment.x1 > segment.x2)
    {
        std::swap(segment.x1, segment.x2);
        std::swap(segment.y1, segment.y2);
    }

    const float dx = segment.x2 - segment.x1;
    const float dy = segment.y2 - segment.y1;
    const float k = (dx < 0.01) ? 1 : dy / dx;

    int x_start;
    float intery;
    {
        const float xend = round(segment.x1);
        const float yend = segment.y1 + k * (xend - segment.x1);
        const float xgap = rfpart(segment.x1 + 0.5);
        x_start = int(xend);
        const int y_end = ipart(yend);
        if (step)
        {
            plot(y_end, x_start, rfpart(yend) * xgap);
            plot(y_end + 1, x_start, fpart(yend) * xgap);
        }
        else
        {
            plot(x_start, y_end, rfpart(yend) * xgap);
            plot(x_start, y_end + 1, fpart(yend) * xgap);
        }
        intery = yend + k;
    }

    int x_end;
    {
        const float xend = round(segment.x2);
        const float yend = segment.y2 + k * (xend - segment.x2);
        const float xgap = rfpart(segment.x2 + 0.5);
        x_end = int(xend);
        const int y_end = ipart(yend);
        if (step)
        {
            plot(y_end, x_end, rfpart(yend) * xgap);
            plot(y_end + 1, x_end, fpart(yend) * xgap);
        }
        else
        {
            plot(x_end, y_end, rfpart(yend) * xgap);
            plot(x_end, y_end + 1, fpart(yend) * xgap);
        }
    }

    if (step)
    {
        for (int x = x_start + 1; x < x_end; x++)
        {
            plot(ipart(intery), x, rfpart(intery));
            plot(ipart(intery) + 1, x, fpart(intery));
            intery += k;
        }
    }
    else
    {
        for (int x = x_start + 1; x < x_end; x++)
        {
            plot(x, ipart(intery), rfpart(intery));
            plot(x, ipart(intery) + 1, fpart(intery));
            intery += k;
        }
    }

    stopTiming(std::abs(dx) + 1.0, std::abs(dy) + 1.0);
}
