#pragma once

#include <functional>
#include "widgetoptions.hpp"
#include "interactivetabwidget.hpp"
#include "ui_canvastabwidget.h"

#include "tabblocks.hpp"

namespace ui
{
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

    protected:
        CanvasDrawerItem* canvasDrawer;

    private:
        Ui::canvasTabWidget ui;

        QGraphicsScene canvasScene;
    };
}
