#pragma once

#include <QMouseEvent>
#include "../../core/cutter/coencutter.hpp"
#include "cuttertab.hpp"

using namespace ui;


CutterTab::CutterTab()
    : InteractiveTabWidget(u8"отсечение (прям.)")
{
    ui.setupUi(this);

    // setup canvas
    canvas = new RectCutterCanvas();
    ui.canvasGroup->layout()->addWidget(canvas);

    connect(ui.cutButton, &QPushButton::pressed, canvas, &RectCutterCanvas::applyCut);
    connect(ui.selectCutter, &QPushButton::pressed, canvas, &RectCutterCanvas::selectCutter);
    connect(canvas, &RectCutterCanvas::cutterSelected, this, &CutterTab::cutterSelected);
}

void ui::CutterTab::constraintToggled(bool checked)
{
}

void ui::CutterTab::cutterSelected(core::Rect& rect)
{
    ui.xLeftInput->setValue(rect.left);
    ui.xRightInput->setValue(rect.left + rect.width);
    ui.yTopInput->setValue(rect.top);
    ui.yBottomInput->setValue(rect.top + rect.height);
}

void ui::CutterTab::clearButtonPressed()
{
    // canvas->clearCanvas();
}

void ui::CutterTab::removeSegmentsButtonPressed()
{
}
