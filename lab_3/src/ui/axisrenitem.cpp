#include "axisrenitem.hpp"

#include <QPainter>

void AxisRenItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QFont font = painter->font();
    font.setPixelSize(12);
    painter->setFont(font);

    painter->drawText(QPointF(4, 16), "0");

    for (int x = boundingBox.left() + 25; x < boundingBox.right(); x += 25)
    {
        painter->drawLine(x, 0, x, x % 100 ? 4 : 12);
        if (x % 100 == 0)
            painter->drawText(QPointF(x + 2, 12 + 4), QString::number(x));
    }
    for (int y = boundingBox.top() + 25; y < boundingBox.bottom(); y += 25)
    {
        painter->drawLine(0, y, y % 100 ? 4 : 12, y);
        if (y % 100 == 0)
            painter->drawText(QPointF(4, y - 2), QString::number(y));
    }
}
