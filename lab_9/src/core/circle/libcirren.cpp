#include <QPainter>
#include "libcirren.hpp"

void core::LibraryCircleRenderer::draw(QImage& image, const Circle& circle, QColor color)
{
    QPainter painter(&image);
    painter.setPen(color);

    beginTiming();
    painter.drawEllipse(circle.getBoundingBox());
    endTiming();
}
