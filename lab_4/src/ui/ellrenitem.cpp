#include <QPainter>
#include "ellrenitem.hpp"

void EllRenItem::addEllipse(double x, double y, double width, double height, QColor color, std::shared_ptr<EllipseRenderer> algorithm)
{
    ellipses.push_back({
        { x, y, width, height, color },
        algorithm
    });
}

void EllRenItem::clearEllipses()
{
    ellipses.clear();
}

void EllRenItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPixmap pixmap(boundingBox.width(), boundingBox.height());
    pixmap.fill();

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (const auto& pair : ellipses)
        pair.second->drawEllipse(image, pair.first);

    painter->drawImage(boundingBox.topLeft(), image);
}
