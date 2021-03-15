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
            255
        );
        image.setPixel(x, y, clr.rgba());
    };

    startTiming();

    const bool steep = abs(segment.y2 - segment.y1) > abs(segment.x2 - segment.x1);
    if (steep)
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
    const float gradient = (dx < 0.01) ? 1 : dy / dx;

    int xpx11;
    float intery;
    {
        const float xend = round(segment.x1);
        const float yend = segment.y1 + gradient * (xend - segment.x1);
        const float xgap = rfpart(segment.x1 + 0.5);
        xpx11 = int(xend);
        const int ypx11 = ipart(yend);
        if (steep) {
            plot(ypx11, xpx11, rfpart(yend) * xgap);
            plot(ypx11 + 1, xpx11, fpart(yend) * xgap);
        }
        else {
            plot(xpx11, ypx11, rfpart(yend) * xgap);
            plot(xpx11, ypx11 + 1, fpart(yend) * xgap);
        }
        intery = yend + gradient;
    }

    int xpx12;
    {
        const float xend = round(segment.x2);
        const float yend = segment.y2 + gradient * (xend - segment.x2);
        const float xgap = rfpart(segment.x2 + 0.5);
        xpx12 = int(xend);
        const int ypx12 = ipart(yend);
        if (steep) {
            plot(ypx12, xpx12, rfpart(yend) * xgap);
            plot(ypx12 + 1, xpx12, fpart(yend) * xgap);
        }
        else {
            plot(xpx12, ypx12, rfpart(yend) * xgap);
            plot(xpx12, ypx12 + 1, fpart(yend) * xgap);
        }
    }

    if (steep) {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            plot(ipart(intery), x, rfpart(intery));
            plot(ipart(intery) + 1, x, fpart(intery));
            intery += gradient;
        }
    }
    else
    {
        for (int x = xpx11 + 1; x < xpx12; x++)
        {
            plot(x, ipart(intery), rfpart(intery));
            plot(x, ipart(intery) + 1, fpart(intery));
            intery += gradient;
        }
    }

    stopTiming(std::abs(dx) + 1, std::abs(dy) + 1);
}
