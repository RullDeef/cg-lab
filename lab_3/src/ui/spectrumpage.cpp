#include "spectrumpage.hpp"
#include <cmath>
#include <QMouseEvent>
#include <QColorDialog>

ui::SpectrumPage::SpectrumPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.colorInput->setScene(&colorScene);
    colorScene.setBackgroundBrush(colorFg);

    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    ui.canvas->setScene(&canvasScene);

    segmentRenderer = new SegmentRenderer();
    canvasScene.addItem(segmentRenderer);

    connect(ui.lengthInput, SIGNAL(valueChanged(int)), this, SLOT(setLength(int)));
    connect(ui.amountInput, SIGNAL(valueChanged(int)), this, SLOT(setAmount(int)));
    connect(ui.stepInput, SIGNAL(valueChanged(int)), this, SLOT(setAngleStep(int)));

    connect(ui.algorithm1Input, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm1(int)));
    connect(ui.algorithm2Input, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlgorithm2(int)));

    connect(ui.draw1Button, SIGNAL(clicked()), this, SLOT(drawSpectrum1()));
    connect(ui.draw2Button, SIGNAL(clicked()), this, SLOT(drawSpectrum2()));
    connect(ui.clearButton, SIGNAL(clicked()), this, SLOT(clearCanvas()));
}

ui::SpectrumPage::~SpectrumPage()
{
    delete segmentRenderer;
}

void ui::SpectrumPage::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui.colorInput->mapFromGlobal(event->globalPos());
    if (ui.colorInput->rect().contains(remapped))
        selectColor();
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
    segmentRenderer->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    segmentRenderer->clearSegments();

    canvasScene.update();
    ui.canvas->repaint();
}

void ui::SpectrumPage::drawSpectrum(SegmentRenderer::Algorithm algorithm, QColor color)
{
    canvasScene.setSceneRect(0, 0, ui.canvas->width(), ui.canvas->height());
    segmentRenderer->setViewport(0, 0, ui.canvas->width(), ui.canvas->height());

    double x1 = segmentRenderer->boundingRect().center().x();
    double y1 = segmentRenderer->boundingRect().center().y();

    for (int angleDeg = 0; angleDeg + 0.5 * angleStep < 360; angleDeg += angleStep)
    {
        double angle = angleDeg * 3.1415926535 / 180.0;

        double x2 = x1 + segmentsLength * cos(angle);
        double y2 = y1 + segmentsLength * sin(angle);

        segmentRenderer->addSegment(x1, y1, x2, y2, color, algorithm);
    }

    canvasScene.update();
    ui.canvas->repaint();
}
