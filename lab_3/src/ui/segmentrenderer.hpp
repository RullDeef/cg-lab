#pragma once

#include <list>
#include <QPainter>
#include <QGraphicsItem>

namespace ui
{
    class SegmentRenderer : public QGraphicsItem
    {
    public:
        enum class Algorithm
        {
            DDA = 0,
            BresenhemReal = 1,
            BresenhemInt = 2,
            BresenhemExt = 3,
            Library = 4
        };

        struct Segment
        {
            double x1, y1, x2, y2;
            QColor color;
            Algorithm algorithm;
        };

        SegmentRenderer();

        void setViewport(int left, int top, int width, int height) { boundingBox = QRectF(left, top, width, height); }
        QRectF boundingRect() const { return boundingBox; }

        void addSegment(double x1, double y1, double x2, double y2, QColor color, Algorithm algorithm);
        void clearSegments();

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    private:
        void renderSegment(QPainter* painter, QRgb* pixels, const Segment& segment);

        void bresenhemRealRender(QRgb* pixels, Segment segment);
        void libraryRender(QPainter* painter, Segment segment);

    private:
        QRectF boundingBox;

        std::list<Segment> segments;
    };
}
