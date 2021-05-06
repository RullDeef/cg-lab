#include <cmath>
#include "wusegren.hpp"

void core::WuSegmentRenderer::draw(QImage& image, const Segment& segment, QColor color)
{
    Segment seg = segment;

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

    beginTiming();

    const bool step = abs(seg.y2 - seg.y1) > abs(seg.x2 - seg.x1);
    if (step)
    {
        std::swap(seg.x1, seg.y1);
        std::swap(seg.x2, seg.y2);
    }
    if (seg.x1 > seg.x2)
    {
        std::swap(seg.x1, seg.x2);
        std::swap(seg.y1, seg.y2);
    }

    const float dx = seg.x2 - seg.x1;
    const float dy = seg.y2 - seg.y1;
    const float k = (dx < 0.01) ? 1 : dy / dx;

    int x_start;
    float intery;
    {
        const float xend = round(seg.x1);
        const float yend = seg.y1 + k * (xend - seg.x1);
        const float xgap = rfpart(seg.x1 + 0.5);
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
        const float xend = round(seg.x2);
        const float yend = seg.y2 + k * (xend - seg.x2);
        const float xgap = rfpart(seg.x2 + 0.5);
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

    endTiming();
}
