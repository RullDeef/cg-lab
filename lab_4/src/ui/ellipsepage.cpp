#include <QMouseEvent>
#include <QColorDialog>
#include "ellrenitem.hpp"
#include "ellipsepage.hpp"

EllipsePage::EllipsePage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.colorInput->setScene(&colorScene);
    colorScene.setBackgroundBrush(color);

    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    ui.canvas->setScene(&canvasScene);

    ellRenItem = new EllRenItem();
    canvasScene.addItem(ellRenItem);

    connect(ui.xInput, SIGNAL(valueChanged(int)), this, SLOT(setEX(int)));
    connect(ui.yInput, SIGNAL(valueChanged(int)), this, SLOT(setEY(int)));
    connect(ui.widthInput, SIGNAL(valueChanged(int)), this, SLOT(setEWidth(int)));
    connect(ui.heightInput, SIGNAL(valueChanged(int)), this, SLOT(setEHeight(int)));

    connect(ui.algorithmInput, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm(int)));

    connect(ui.drawButton, SIGNAL(clicked()), this, SLOT(drawEllipse()));
    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(clearCanvas()));
}

void EllipsePage::initAlgos(std::vector<std::shared_ptr<EllipseRenderer>> algos)
{
    ellipseRenderers = algos;

    // setup algorithm dropdown list
    ui.algorithmInput->clear();
    for (auto& renderer : ellipseRenderers)
        ui.algorithmInput->addItem(renderer->getName());
}

void EllipsePage::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui.colorInput->mapFromGlobal(event->globalPos());
    if (ui.colorInput->rect().contains(remapped))
        selectColor();
}

void EllipsePage::selectColor()
{
    QColorDialog dialog;

    if (dialog.exec())
    {
        color = dialog.selectedColor();
        colorScene.setBackgroundBrush(color);
    }
}

void EllipsePage::drawEllipse()
{
    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    ellRenItem->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    ellRenItem->addEllipse(eXCenter, eYCenter, eWidth, eHeight, color, ellipseRenderers[algorithmIndex]);

    canvasScene.update();
    ui.canvas->repaint();
}

void EllipsePage::clearCanvas()
{
    ellRenItem->clearEllipses();

    canvasScene.update();
    ui.canvas->repaint();
}
