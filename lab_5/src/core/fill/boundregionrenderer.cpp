#include <cmath>
#include "boundregionrenderer.hpp"

void core::AsyncBoundRegionRenderer::fill(QImage& image, const BasicRegion& region, QColor color)
{
    beginTiming();

    if (region.getLines().empty())
    {
        endTiming();
        return;
    }

    QColor backgroud = Qt::white;

    int boundX = 0;
    for (const auto& point : region.getPoints()) boundX += point.x;
    boundX /= region.getPoints().size();

    for (const auto& line : region.getLines())
    {
        if (line.isHorizontal())
            continue;

        int x_min = line.p1->x, y_min = line.p1->y;
        int x_max = line.p2->x, y_max = line.p2->y;

        if (y_min > y_max)
        {
            std::swap(x_min, x_max);
            std::swap(y_min, y_max);
        }

        double xReal = x_min;
        double dx = double(x_max - x_min) / (y_max - y_min);

        for (int y = y_min; y < y_max; y++, xReal += dx)
        {
            for (int x = std::round(xReal); x < boundX; x++)
            {
                if (image.pixel(x, y) != color.rgba())
                    image.setPixel(x, y, color.rgba());
                else
                    image.setPixel(x, y, backgroud.rgba());
            }

            for (int x = std::round(xReal); x >= boundX; x--)
            {
                if (image.pixel(x, y) != color.rgba())
                    image.setPixel(x, y, color.rgba());
                else
                    image.setPixel(x, y, backgroud.rgba());
            }
        }
    }

    endTiming();
}

void core::AsyncBoundRegionRenderer::asyncFill(QImage& image, const BasicRegion& region, QColor color)
{
    if (region.getLines().empty())
    {
        finish();
        return;
    }

    if (!initialized)
    {
        currentRegion = region.clone();

        boundX = 0;
        for (const auto& point : currentRegion.getPoints()) boundX += point.x;
        boundX /= region.getPoints().size();

        currentLine = currentRegion.getLines().begin();
        currentY = std::min(currentLine->p1->y, currentLine->p2->y);

        initialized = true;
    }
    else if (currentLine == currentRegion.getLines().end())
        finish();
    else
    {
        QColor backgroud = Qt::white;
        if (currentLine->isHorizontal())
        {
            if (++currentLine == currentRegion.getLines().end())
                finish();
            else
                currentY = std::min(currentLine->p1->y, currentLine->p2->y);

            return;
        }

        const auto& line = *currentLine;

        int x_min = line.p1->x, y_min = line.p1->y;
        int x_max = line.p2->x, y_max = line.p2->y;

        if (y_min > y_max)
        {
            std::swap(x_min, x_max);
            std::swap(y_min, y_max);
        }

        double xReal = x_min + double(x_max - x_min) / (y_max - y_min) * (currentY - y_min);

        if (currentY < y_max)
        {
            for (int x = std::round(xReal); x < boundX; x++)
            {
                if (image.pixel(x, currentY) != color.rgba())
                    image.setPixel(x, currentY, color.rgba());
                else
                    image.setPixel(x, currentY, backgroud.rgba());
            }

            for (int x = std::round(xReal); x >= boundX; x--)
            {
                if (image.pixel(x, currentY) != color.rgba())
                    image.setPixel(x, currentY, color.rgba());
                else
                    image.setPixel(x, currentY, backgroud.rgba());
            }

            currentY++;
        }
        else
        {
            if (++currentLine == currentRegion.getLines().end())
                finish();
            else
                currentY = std::min(currentLine->p1->y, currentLine->p2->y);
        }
    }
}
