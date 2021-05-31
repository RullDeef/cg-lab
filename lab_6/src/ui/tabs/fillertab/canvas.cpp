#include "canvas.hpp"

using namespace ui;

CanvasRendererItem::CanvasRendererItem(QSize size)
    : image(QPixmap(size).toImage())
{
    image.convertTo(QImage::Format::Format_ARGB32);
    image.fill(Qt::white);
}

void CanvasRendererItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawImage(0, 0, image);
}

void CanvasRendererItem::resizeImage(QSize size)
{
    if (image.size() != size)
    {
        image = QPixmap(size).toImage();
        image.convertTo(QImage::Format::Format_ARGB32);
        image.fill(Qt::white);
    }
}


Canvas::Canvas() : canvasRenderer(new CanvasRendererItem)
{
    setScene(&scene);
    scene.addItem(canvasRenderer);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    canvasRenderer->resizeImage(size());
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
    canvasRenderer->resizeImage(size());

    scene.setSceneRect(QRectF(QPointF(0, 0), size()));
    scene.update();
}
