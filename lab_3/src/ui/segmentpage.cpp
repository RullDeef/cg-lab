#include <QMessageBox>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QgraphicsView>
#include <QMouseEvent>
#include "segmentpage.hpp"

ui::SegmentPage::SegmentPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.colorInput->setScene(&colorScene);
    colorScene.setBackgroundBrush(color);

    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    ui.canvas->setScene(&canvasScene);

    segRenItem = new SegRenItem();
    canvasScene.addItem(segRenItem);
    
    connect(ui.x1Input, SIGNAL(valueChanged(int)), this, SLOT(setX1(int)));
    connect(ui.y1Input, SIGNAL(valueChanged(int)), this, SLOT(setY1(int)));
    connect(ui.x2Input, SIGNAL(valueChanged(int)), this, SLOT(setX2(int)));
    connect(ui.y2Input, SIGNAL(valueChanged(int)), this, SLOT(setY2(int)));

    connect(ui.algorithmInput, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm(int)));

    connect(ui.drawButton, SIGNAL(clicked()), this, SLOT(drawSegment()));
    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(clearCanvas()));
}

ui::SegmentPage::~SegmentPage()
{
    delete segRenItem;
}

void ui::SegmentPage::initAlogs(const std::list<core::SegmentRenderer*>& algos)
{
    segmentRenderers.clear();
    std::copy(algos.begin(), algos.end(), std::back_inserter(segmentRenderers));

    // setup algorithm dropdown list
    ui.algorithmInput->clear();
    for (auto& renderer : segmentRenderers)
        ui.algorithmInput->addItem(renderer->getName());
}

void ui::SegmentPage::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui.colorInput->mapFromGlobal(event->globalPos());
    if (ui.colorInput->rect().contains(remapped))
        selectColor();
}

void ui::SegmentPage::selectColor()
{
    QColorDialog dialog;

    if (dialog.exec())
    {
        color = dialog.selectedColor();
        colorScene.setBackgroundBrush(color);
    }
}

void ui::SegmentPage::drawSegment()
{
    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    segRenItem->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    segRenItem->addSegment(x1, y1, x2, y2, color, segmentRenderers[algorithmIndex]);

    canvasScene.update();
    ui.canvas->repaint();
}

void ui::SegmentPage::clearCanvas()
{
    segRenItem->clearSegments();

    canvasScene.update();
    ui.canvas->repaint();
}
