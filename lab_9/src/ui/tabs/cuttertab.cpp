#pragma once

#include <QMouseEvent>
#include <QMessageBox>
#include "../../core/cutter/coencutter.hpp"
#include "cuttertab.hpp"

using namespace ui;


CutterTab::CutterTab()
    : InteractiveTabWidget(u8"отсечение (произв.)")
{
    ui.setupUi(this);

    // setup canvas
    canvas = new RectCutterCanvas();
    ui.canvasGroup->layout()->addWidget(canvas);

    connect(ui.cutButton, &QPushButton::pressed, canvas, &RectCutterCanvas::applyCut);
    connect(ui.selectCutter, &QPushButton::pressed, canvas, &RectCutterCanvas::selectCutter);
    connect(ui.addRegionButton, &QPushButton::pressed, canvas, &RectCutterCanvas::addRegion);
    connect(canvas, &RectCutterCanvas::cutterSelected, this, &CutterTab::cutterSelected);
    connect(ui.removeRegionButton, &QPushButton::pressed, this, &CutterTab::removeRegionButtonPressed);
    connect(ui.clearButton, &QPushButton::pressed, canvas, &RectCutterCanvas::clearCut);
    connect(ui.deleteCutterButton, &QPushButton::pressed, canvas, &RectCutterCanvas::removeCutter);
}

void ui::CutterTab::removeRegionButtonPressed()
{
    canvas->removeRegion();
}

void ui::CutterTab::cutterSelected(core::BasicRegion& cutter)
{
    if (!canvas->cutterIsValid())
    {
        QMessageBox::critical(this, u8"Ошибка", u8"Некорректный отсекатель. Пожалуйста, введите несамопересекающийся отсекатель.", QMessageBox::Button::Ok);
        cutter.clear();
    }
}

void ui::CutterTab::contourAdded(core::BasicRegion& region)
{
    if (region.selfIntersecting())
    {
        QMessageBox::critical(this, u8"Ошибка", u8"Некорректный Многоугольник. Введите несамопересекающийся многоугольник.", QMessageBox::Button::Ok);
        region.clear();
    }
}
