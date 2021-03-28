#include <QMouseEvent>
#include <QColorDialog>
#include "cirrenitem.hpp"
#include "circlepage.hpp"

CirclePage::CirclePage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.colorInput->setScene(&colorScene);
    colorScene.setBackgroundBrush(color);

    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    ui.canvas->setScene(&canvasScene);

    cirRenItem = new CirRenItem();
    canvasScene.addItem(cirRenItem);

    connect(ui.xInput, SIGNAL(valueChanged(int)), this, SLOT(setCX(int)));
    connect(ui.yInput, SIGNAL(valueChanged(int)), this, SLOT(setCY(int)));
    connect(ui.radiusInput, SIGNAL(valueChanged(int)), this, SLOT(setCRadius(int)));

    connect(ui.algorithmInput, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm(int)));

    connect(ui.drawButton, SIGNAL(clicked()), this, SLOT(drawCircle()));
    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(clearCanvas()));
}

void CirclePage::initAlgos(std::vector<std::shared_ptr<CircleRenderer>> algos)
{
    circleRenderers = algos;

    // setup algorithm dropdown list
    ui.algorithmInput->clear();
    for (auto& renderer : circleRenderers)
        ui.algorithmInput->addItem(renderer->getName());
}

void CirclePage::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui.colorInput->mapFromGlobal(event->globalPos());
    if (ui.colorInput->rect().contains(remapped))
        selectColor();
}

void CirclePage::selectColor()
{
    QColorDialog dialog;

    if (dialog.exec())
    {
        color = dialog.selectedColor();
        colorScene.setBackgroundBrush(color);
    }
}

void CirclePage::drawCircle()
{
    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    cirRenItem->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    cirRenItem->addCircle(eXCenter, eYCenter, eRadius, color, circleRenderers[algorithmIndex]);

    canvasScene.update();
    ui.canvas->repaint();
}

void CirclePage::clearCanvas()
{
    cirRenItem->clearCircles();

    canvasScene.update();
    ui.canvas->repaint();
}
