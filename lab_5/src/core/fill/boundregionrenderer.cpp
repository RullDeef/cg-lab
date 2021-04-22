#include <cmath>
#include "boundregionrenderer.hpp"

void core::BoundRegionRenderer::fill(QImage& image, const BasicRegion& region, QColor color)
{
    QColor backgroud = Qt::white;
    QRectF box = region.getBoundingBox();

    int minY = std::round(box.top());
    int maxY = std::round(box.bottom());

    int boundX = 0;
    for (const auto& point : region.getPoints()) boundX += point.x;
    boundX /= region.getPoints().size();

    for (const auto& line : region.getLines())
    {
        if (line.isHorizontal())
            continue;

        double dx = double(line.p2->x - line.p1->x) / (line.p2->y - line.p1->y);
        double xReal = line.p1->x;

        int yDir = line.p1->y < line.p2->y ? 1 : -1;
        for (int y = line.p1->y; y <= line.p2->y; y += yDir, xReal += dx)
        {
            for (int x = std::round(xReal); x < boundX; x++)
            {
                if (image.pixel(x, y) == backgroud.rgba())
                    image.setPixel(x, y, color.rgba());
                else
                    image.setPixel(x, y, backgroud.rgba());
            }

            for (int x = std::round(xReal); x >= boundX; x--)
            {
                if (image.pixel(x, y) == backgroud.rgba())
                    image.setPixel(x, y, color.rgba());
                else
                    image.setPixel(x, y, backgroud.rgba());
            }
        }
    }
}
