#include "fillertab.hpp"

#include <QFormLayout>
#include <QCheckBox>
#include <chrono>

#include "../../core/fill/bucfillregionrenderer.hpp"

using namespace core;
using namespace ui;


FillerTab::FillerTab(AsyncBucFillRenderer* renderer)
    : InteractiveTabWidget(u8"заполнение"), renderer(renderer)
{
    ui.setupUi(this);

    // setup canvas
    canvas = new SmartCanvas();
    ui.canvasGroup->layout()->addWidget(canvas);

    // hidePointEditor();

    // canvas
    // connect(canvas, &SmartCanvas::selectionChanged, this, &FillerTab::selectionChangedSlot);

    // special point handling
    connect(ui.selectFillPointButton, &QPushButton::clicked, canvas, &SmartCanvas::specialPointSelectAction);
    connect(canvas, &SmartCanvas::specialPointPosChanged, this, &FillerTab::specialPointChanged);
    connect(ui.fillXInput, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [&](int x) { canvas->changeSpecialPointPos(x, ui.fillYInput->value()); });
    connect(ui.fillYInput, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [&](int y) { canvas->changeSpecialPointPos(ui.fillXInput->value(), y); });

    // checkboxes
    // connect(ui.constraintCheckBox, &QCheckBox::toggled, this, &FillerTab::constraintToggled);
    // connect(ui.linesCheckBox, &QCheckBox::toggled, this, &FillerTab::lineToggled);

    // connect(canvas, &SmartCanvas::constraintToggled, ui.constraintCheckBox, &QCheckBox::setChecked);
    // connect(canvas, &SmartCanvas::lineToggled, ui.linesCheckBox, &QCheckBox::setChecked);

    // color picker
    colorPicker = new ColorPickerWidget;
    colorPicker->setColor(Qt::green);
    ui.colorForm->setWidget(0, QFormLayout::ItemRole::FieldRole, colorPicker);
    connect(colorPicker, &ColorPickerWidget::colorChanged, this, &FillerTab::colorPicked);
    colorPicked(colorPicker->getColor());

    // buttons
    connect(ui.fillButton, &QPushButton::clicked, this, &FillerTab::fillButtonPressed);
    connect(ui.fillStepButton, &QPushButton::clicked, this, &FillerTab::fillStepButtonPressed);
    connect(ui.clearFillButton, &QPushButton::clicked, this, &FillerTab::clearOverlayButtonPressed);
    connect(ui.removeRegionButton, &QPushButton::clicked, this, &FillerTab::removeRegionButtonPressed);
    connect(ui.clearButton, &QPushButton::clicked, this, &FillerTab::clearButtonPressed);
}

void FillerTab::specialPointChanged(int x, int y)
{
    ui.fillXInput->setValue(x);
    ui.fillYInput->setValue(y);
}

void FillerTab::fillButtonPressed()
{
    auto startTime = std::chrono::system_clock::now();
    canvas->fillRegion(core::BucFillRegionRenderer(), colorPicker->getColor());
    auto endTime = std::chrono::system_clock::now();

    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    ui.fillTime->setText(QString(u8"Время заполнения: ") + QString::number(delta) + u8" мкс");
}

void FillerTab::fillStepButtonPressed()
{
    canvas->fillRegionWithStep(new core::AsyncBucFillRegionRenderer, colorPicker->getColor());
    ui.fillTime->setText(QString(u8"Время заполнения: --"));
}

void FillerTab::clearButtonPressed()
{
    canvas->clearRegion();
    canvas->clearCanvas();
}

void ui::FillerTab::removeRegionButtonPressed()
{
    canvas->clearRegion();
    canvas->update();
}

void ui::FillerTab::clearOverlayButtonPressed()
{
    canvas->clearOverlay();
}

void ui::FillerTab::colorPicked(QColor color)
{
    canvas->setColor(color);
}
