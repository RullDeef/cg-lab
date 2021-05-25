#include "spectrumpage.hpp"
#include <cmath>
#include <QMouseEvent>
#include <QColorDialog>

using namespace core;

ui::SpectrumPage::SpectrumPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.colorInput->setScene(&colorScene);
    colorScene.setBackgroundBrush(colorFg);

    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    ui.canvas->setScene(&canvasScene);

    segRenItem = new SegRenItem();
    canvasScene.addItem(segRenItem);

    axisRenItem = new AxisRenItem();
    canvasScene.addItem(axisRenItem);

    connect(ui.lengthInput, SIGNAL(valueChanged(int)), this, SLOT(setLength(int)));
    connect(ui.amountInput, SIGNAL(valueChanged(int)), this, SLOT(setAmount(int)));

    connect(ui.algorithm1Input, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm1(int)));
    connect(ui.algorithm2Input, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm2(int)));

    connect(ui.draw1Button, SIGNAL(clicked()), this, SLOT(drawSpectrum1()));
    connect(ui.draw2Button, SIGNAL(clicked()), this, SLOT(drawSpectrum2()));
    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(clearCanvas()));
}

ui::SpectrumPage::~SpectrumPage()
{
    delete segRenItem;
    delete axisRenItem;
}

void ui::SpectrumPage::initAlgos(const std::list<core::SegmentRenderer*>& algos)
{
    segmentRenderers.clear();
    std::copy(algos.begin(), algos.end(), std::back_inserter(segmentRenderers));

    // setup algorithm dropdown list
    ui.algorithm1Input->clear();
    ui.algorithm2Input->clear();
    for (auto& renderer : segmentRenderers)
    {
        ui.algorithm1Input->addItem(renderer->getName());
        ui.algorithm2Input->addItem(renderer->getName());
    }
}

void ui::SpectrumPage::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui.colorInput->mapFromGlobal(event->globalPos());
    if (ui.colorInput->rect().contains(remapped))
        selectColor();
}

void ui::SpectrumPage::paintEvent(QPaintEvent* event)
{
    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    segRenItem->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());
    axisRenItem->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    canvasScene.update();
}

void ui::SpectrumPage::selectColor()
{
    QColorDialog dialog;

    if (dialog.exec())
    {
        colorFg = dialog.selectedColor();
        colorScene.setBackgroundBrush(colorFg);
    }
}

void ui::SpectrumPage::clearCanvas()
{
    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    segRenItem->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    segRenItem->clearSegments();

    canvasScene.update();
    ui.canvas->repaint();
}

void ui::SpectrumPage::drawSpectrum(SegmentRenderer* renderer, QColor color)
{
    double x1 = segRenItem->boundingRect().center().x();
    double y1 = segRenItem->boundingRect().center().y();

    constexpr auto pi = 3.14159265358979323846;
    constexpr auto tau = 2 * pi;

    double angleStep = tau / segmentsAmount;
    for (double angle = 0.0; angle < tau; angle += angleStep)
    {
        double x2 = x1 + segmentsLength * cos(angle);
        double y2 = y1 + segmentsLength * sin(angle);

        segRenItem->addSegment(x1, y1, x2, y2, color, renderer);
    }

    canvasScene.update();
    ui.canvas->repaint();
}
