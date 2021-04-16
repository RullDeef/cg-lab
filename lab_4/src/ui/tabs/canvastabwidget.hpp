#pragma once

#include <functional>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "widgetoptions.hpp"
#include "interactivetabwidget.hpp"
#include "ui_canvastabwidget.h"

namespace ui
{
    class CanvasDrawerItem : public QGraphicsItem
    {
    public:
        CanvasDrawerItem(QSize size = QSize(1, 1)) : image(QPixmap(size).toImage())
        {
            image.convertTo(QImage::Format::Format_ARGB32);
            image.fill(Qt::white);

            renderAxis();
        }

        void resizeImage(QSize size)
        {
            if (image.size() != size)
            {
                image = QPixmap(size).toImage();
                image.convertTo(QImage::Format::Format_ARGB32);
                image.fill(Qt::white);

                renderAxis();
            }
        }

        QRectF boundingRect() const override { return QRectF(QPointF(0, 0), image.size()); }
        QImage& getImage() { return image; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR) override
        {
            renderAxis();
            painter->drawImage(0, 0, image);
        }

    private:
        void renderAxis()
        {
            QPainter painter(&image);

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

        QImage image;
    };

    class CanvasTabWidget : public InteractiveTabWidget, public WidgetOptionsStrategy
    {
        Q_OBJECT

    public:
        CanvasTabWidget(const char* name, QWidget* parent = Q_NULLPTR)
            : InteractiveTabWidget(name, parent)
        {
            ui.setupUi(this);
            initOptionStrategy(ui.optionsGroup, ui.inputForm);

            ui.canvas->setScene(&canvasScene);
            canvasDrawer = new CanvasDrawerItem();
            canvasScene.addItem(canvasDrawer);
        }

        ~CanvasTabWidget()
        {
            delete canvasDrawer;
        }

        void paintEvent(QPaintEvent* event)
        {
            InteractiveTabWidget::paintEvent(event);
            canvasDrawer->resizeImage(ui.canvas->size());

            canvasScene.setSceneRect(QRectF(QPointF(0, 0), ui.canvas->size()));
            canvasScene.update();
        }

        void updateCanvas()
        {
            canvasScene.setSceneRect(QRectF(QPointF(0, 0), ui.canvas->size()));
            canvasDrawer->update();
            canvasScene.update();
            ui.canvas->repaint();
        }

    protected:
        CanvasDrawerItem* canvasDrawer;

    private:
        Ui::canvasTabWidget ui;

        QGraphicsScene canvasScene;
    };
}
