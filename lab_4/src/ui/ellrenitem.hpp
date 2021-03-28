#pragma once

#include <QGraphicsItem>
#include <memory>
#include "../core/ellipserenderer.hpp"

class EllRenItem : public QGraphicsItem
{
public:
    void setViewport(int left, int top, int width, int height) { boundingBox = QRectF(left, top, width, height); }
    QRectF boundingRect() const { return boundingBox; }

    void addEllipse(double x, double y, double width, double height, QColor color, std::shared_ptr<EllipseRenderer> algorithm);
    void clearEllipses();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QRectF boundingBox;

    std::list<std::pair<Ellipse, std::shared_ptr<EllipseRenderer>>> ellipses;
};

