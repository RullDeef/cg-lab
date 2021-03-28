#pragma once

#include <QGraphicsItem>
#include <memory>
#include "../core/circlerenderer.hpp"

class CirRenItem : public QGraphicsItem
{
public:
    void setViewport(int left, int top, int width, int height) { boundingBox = QRectF(left, top, width, height); }
    QRectF boundingRect() const { return boundingBox; }

    void addCircle(double x, double y, double radius, QColor color, std::shared_ptr<CircleRenderer> algorithm);
    void clearCircles();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QRectF boundingBox;

    std::list<std::pair<Circle, std::shared_ptr<CircleRenderer>>> circles;
};

