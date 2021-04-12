#include "libellren.hpp"

void core::LibraryEllipseRenderer::draw(QImage& image, const Ellipse& ellipse, QColor color)
{
    QPainter painter(&image);
    painter.setPen(color);

    beginTiming();
    painter.drawEllipse(ellipse.getBoundingBox());
    endTiming();
}
