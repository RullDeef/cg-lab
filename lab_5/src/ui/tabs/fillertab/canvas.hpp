#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace ui
{
    class CanvasRendererItem : public QGraphicsItem
    {
    public:
        CanvasRendererItem(QSize size = QSize(1, 1));

        void resizeImage(QSize size);

        QRectF boundingRect() const override { return QRectF(QPointF(0, 0), image.size()); }
        QImage& getImage() { return image; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR) override;

    private:
        QImage image;
    };

    class Canvas : public QGraphicsView
    {
        Q_OBJECT

    public:
        Canvas();
        virtual ~Canvas() { delete canvasRenderer; }

    protected slots:
        virtual void resizeEvent(QResizeEvent* event) override;
        virtual void paintEvent(QPaintEvent* event) override;

    protected:
        QImage& getImage() { return canvasRenderer->getImage(); }

    private:
        QGraphicsScene scene;
        CanvasRendererItem* canvasRenderer;
    };
}
