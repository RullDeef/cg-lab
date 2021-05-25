#include "segrenitem.hpp"
#include <memory>

using namespace core;

void ui::SegRenItem::addSegment(double x1, double y1, double x2, double y2, QColor color, SegmentRenderer* algorithm)
{
    segments.push_back({
        { x1, y1, x2, y2, color },
        algorithm
    });
}

void ui::SegRenItem::clearSegments()
{
    segments.clear();
}

void ui::SegRenItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPixmap pixmap(boundingBox.width(), boundingBox.height());
    pixmap.fill();

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (const auto& pair : segments)
        pair.second->drawSegment(image, pair.first);

    painter->drawImage(boundingBox.topLeft(), image);
}
