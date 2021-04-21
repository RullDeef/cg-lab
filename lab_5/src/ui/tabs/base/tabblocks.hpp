// намечается brand new architecture, но мне лень её реализовывать.
// идея заключается в модульности компонент - блок холста, блок графика, блок полей ввода и тп...

#pragma once

#include <QLayout>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace ui
{
    class CanvasDrawerItem : public QGraphicsItem
    {
    public:
        CanvasDrawerItem(QSize size = QSize(1, 1)) : image(QPixmap(size).toImage())
        {
            image.convertTo(QImage::Format::Format_ARGB32);
            image.fill(Qt::white);
        }

        void resizeImage(QSize size)
        {
            if (image.size() != size)
            {
                image = QPixmap(size).toImage();
                image.convertTo(QImage::Format::Format_ARGB32);
                image.fill(Qt::white);
            }
        }

        QRectF boundingRect() const override { return QRectF(QPointF(0, 0), image.size()); }
        QImage& getImage() { return image; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR) override
        {
            painter->drawImage(0, 0, image);
        }

    private:
        QImage image;
    };

    class CanvasBlock : public QGroupBox
    {
        Q_OBJECT

    public:
        CanvasBlock(QWidget* parent = Q_NULLPTR) : QGroupBox(parent)
        {
            setTitle(u8"холст");

            graphicsView = new QGraphicsView(this);
            layout()->addWidget(graphicsView);

            graphicsView->setScene(&canvasScene);
            canvasDrawer = new CanvasDrawerItem();
            canvasScene.addItem(canvasDrawer);
        }

        ~CanvasBlock()
        {
            delete canvasDrawer;
        }

        void paintEvent(QPaintEvent* event)
        {
            QGroupBox::paintEvent(event);

            canvasDrawer->resizeImage(graphicsView->size());

            canvasScene.setSceneRect(QRectF(QPointF(0, 0), graphicsView->size()));
            canvasScene.update();
        }

    private:
        QGraphicsScene canvasScene;
        QGraphicsView* graphicsView;
        CanvasDrawerItem* canvasDrawer;
    };
}
