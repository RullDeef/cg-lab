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
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

void ui::Canvas::drawAxes(QPainter& painter)
{
    QImage& image = getImage();
    painter.setPen(Qt::black);

    for (int x = 0; x < image.width(); x += 25)
    {
        painter.drawLine(x, 0, x, x % 100 == 0 ? 10 : 6);
        if (x % 100 == 0)
            painter.drawText(x + 4, 10, QString::number(x));
    }

    for (int y = 0; y < image.height(); y += 25)
    {
        painter.drawLine(0, y, y % 100 == 0 ? 10 : 6, y);
        if (y % 100 == 0)
            painter.drawText(4, y, QString::number(y));
    }
}
