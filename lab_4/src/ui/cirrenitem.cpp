#include <QPainter>
#include "cirrenitem.hpp"

void CirRenItem::addCircle(double x, double y, double radius, QColor color, std::shared_ptr<CircleRenderer> algorithm)
{
    circles.push_back({
        { x, y, radius, color },
        algorithm
    });
}

void CirRenItem::clearCircles()
{
    circles.clear();
}

void CirRenItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPixmap pixmap(boundingBox.width(), boundingBox.height());
    pixmap.fill();

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (const auto& pair : circles)
        pair.second->drawCircle(image, pair.first);

    painter->drawImage(boundingBox.topLeft(), image);
}
