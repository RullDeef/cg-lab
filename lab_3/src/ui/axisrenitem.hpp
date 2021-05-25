#pragma once

#include <QGraphicsItem>

class AxisRenItem : public QGraphicsItem
{
public:
    void setViewport(int left, int top, int width, int height) { boundingBox = QRectF(left, top, width, height); }
    QRectF boundingRect() const { return boundingBox; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QRectF boundingBox;
};

