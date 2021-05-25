#pragma once

#include <list>
#include <QPainter>
#include <QGraphicsItem>
#include "../core/segmentrenderer.hpp"

namespace ui
{
    class SegRenItem : public QGraphicsItem
    {
    public:
        void setViewport(int left, int top, int width, int height) { boundingBox = QRectF(left, top, width, height); }
        QRectF boundingRect() const { return boundingBox; }

        void addSegment(double x1, double y1, double x2, double y2, QColor color, core::SegmentRenderer* algorithm);
        void clearSegments();

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    private:
        QRectF boundingBox;

        std::list<std::pair<core::Segment, core::SegmentRenderer*>> segments;
    };
}
