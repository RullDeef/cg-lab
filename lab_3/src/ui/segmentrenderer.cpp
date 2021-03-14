#include "segmentrenderer.hpp"
#include <memory>

ui::SegmentRenderer::SegmentRenderer()
{
}

void ui::SegmentRenderer::addSegment(double x1, double y1, double x2, double y2, QColor color, Algorithm algorithm)
{
    double width = boundingBox.width();
    double height = boundingBox.height();

    // check for invalid/invisible segments

    // clamp end points
    if (x1 < 0.0)
    {
        y1 += (-x1) * (y2 - y1) / (x2 - x1);
        x1 = 0.0;
    }
    else if (x1 >= width)
    {
        y1 += (width - 1 - x1) * (y2 - y1) / (x2 - x1);
        x1 = width - 1;
    }

    if (y1 < 0.0)
    {
        x1 += (-y1) * (x2 - x1) / (y2 - y1);
        y1 = 0.0;
    }
    else if (y1 >= height)
    {
        x1 += (height - 1 - y1) * (x2 - x1) / (y2 - y1);
        y1 = height - 1;
    }

    if (x2 < 0.0)
    {
        y2 += (-x2) * (y1 - y2) / (x1 - x2);
        x2 = 0.0;
    }
    else if (x2 >= width)
    {
        y2 += (width - 1 - x2) * (y1 - y2) / (x1 - x2);
        x2 = width - 1;
    }

    if (y2 < 0.0)
    {
        x2 += (-y2) * (x1 - x2) / (y1 - y2);
        y2 = 0.0;
    }
    else if (y2 >= height)
    {
        x2 += (height - 1 - y2) * (x1 - x2) / (y1 - y2);
        y2 = height - 1;
    }

    segments.push_back({ x1, y1, x2, y2, color, algorithm });
}

void ui::SegmentRenderer::clearSegments()
{
    segments.clear();
}

void ui::SegmentRenderer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // create pixel data
    unsigned int size = boundingBox.width() * boundingBox.height();
    QRgb* pixels = new QRgb[size];

    for (unsigned int i = 0; i < size; i++)
         pixels[i] = QColor(255, 255, 255, 255).rgba();

    for (const auto& segment : segments)
        if (segment.algorithm != Algorithm::Library)
            renderSegment(painter, pixels, segment);

    painter->drawImage(boundingBox.topLeft(), QImage((unsigned char*)pixels,
        boundingBox.width(), boundingBox.height(),
        boundingBox.width() * 4, QImage::Format::Format_ARGB32));

    // WARNING: bad rendering order
    for (const auto& segment : segments)
        if (segment.algorithm == Algorithm::Library)
            renderSegment(painter, pixels, segment);

    delete[] pixels;
}

void ui::SegmentRenderer::renderSegment(QPainter* painter, QRgb* pixels, const Segment& segment)
{
    switch (segment.algorithm)
    {
    case Algorithm::BresenhemReal:
        bresenhemRealRender(pixels, segment);
        break;

    case Algorithm::Library:
        libraryRender(painter, segment);
        break;
    }
}

// 1st & 4th quadrants (x1 < x2)
static void bresenhemRealRenderSection(QRgb* pixels, int width, int x1, int y1, int x2, int y2, QRgb color)
{
    int x = x1;
    int y = y1;

    int deltaX = abs(x2 - x1) + 1;
    int deltaY = abs(y2 - y1) + 1;

    int yDir = y1 < y2 ? 1 : -1;

    double error = 0.0;
    double deltaError;

    if (deltaX < deltaY)
    {
        deltaError = static_cast<double>(deltaX) / deltaY;

        while (y != y2 + yDir)
        {
            pixels[width * y + x] = color;

            error += deltaError;
            if (error >= 1.0)
            {
                error -= 1.0;
                x++;
            }

            y += yDir;
        }
    }
    else
    {
        deltaError = static_cast<double>(deltaY) / deltaX;

        while (x != x2 + 1)
        {
            pixels[width * y + x] = color;

            error += deltaError;
            if (error >= 1.0)
            {
                error -= 1.0;
                y += yDir;
            }

            x++;
        }
    }
}

void ui::SegmentRenderer::bresenhemRealRender(QRgb* pixels, Segment segment)
{
    if (segment.x2 < segment.x1) // swap end points
    {
        std::swap(segment.x1, segment.x2);
        std::swap(segment.y1, segment.y2);
    }

    double deltaX = segment.x2 - segment.x1;
    double deltaY = segment.y2 - segment.y1;

    if (false && abs(deltaX) < 1.0)
    {
        // draw straight line
    }
    else if (false && abs(deltaY) < 1.0)
    {
        // draw straight line
    }
    else
    {
        bresenhemRealRenderSection(pixels, boundingBox.width(), segment.x1, segment.y1, segment.x2, segment.y2, segment.color.rgba());
    }
}

void ui::SegmentRenderer::libraryRender(QPainter* painter, Segment segment)
{
    painter->setPen(QPen(segment.color, 1));
    painter->drawLine(segment.x1, segment.y1, segment.x2, segment.y2);
}
