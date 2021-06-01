#include "fillertab.hpp"

#include <QFormLayout>
#include <QCheckBox>
#include <chrono>

#include "../../core/fill/bucfillregionrenderer.hpp"

using namespace core;
using namespace ui;


FillerTab::FillerTab(RegionRenderer* renderer)
    : InteractiveTabWidget(u8"заполнение"), renderer(renderer), regionWrapper(region)
{
    ui.setupUi(this);

    // setup canvas
    canvas = new SmartCanvas(regionWrapper);
    ui.canvasGroup->layout()->addWidget(canvas);

    hidePointEditor();

    // canvas
    connect(canvas, &SmartCanvas::selectionChanged, this, &FillerTab::selectionChangedSlot);

    // special point handling
    connect(ui.selectFillPointButton, &QPushButton::clicked, canvas, &SmartCanvas::specialPointSelectAction);
    connect(canvas, &SmartCanvas::specialPointPosChanged, this, &FillerTab::specialPointChanged);
    connect(ui.fillXInput, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [&](int x) { canvas->changeSpecialPointPos(x, ui.fillYInput->value()); });
    connect(ui.fillYInput, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [&](int y) { canvas->changeSpecialPointPos(ui.fillXInput->value(), y); });

    // checkboxes
    connect(ui.constraintCheckBox, &QCheckBox::toggled, this, &FillerTab::constraintToggled);
    connect(ui.linesCheckBox, &QCheckBox::toggled, this, &FillerTab::lineToggled);

    connect(canvas, &SmartCanvas::constraintToggled, ui.constraintCheckBox, &QCheckBox::setChecked);
    connect(canvas, &SmartCanvas::lineToggled, ui.linesCheckBox, &QCheckBox::setChecked);

    // color picker
    colorPicker = new ColorPickerWidget;
    colorPicker->setColor(Qt::green);
    ui.colorForm->setWidget(0, QFormLayout::ItemRole::FieldRole, colorPicker);

    // buttons
    connect(ui.fillButton, &QPushButton::clicked, this, &FillerTab::fillButtonPressed);
    connect(ui.fillStepButton, &QPushButton::clicked, this, &FillerTab::fillStepButtonPressed);
    connect(ui.clearFillButton, &QPushButton::clicked, this, &FillerTab::clearOverlayButtonPressed);
    connect(ui.removeRegionButton, &QPushButton::clicked, this, &FillerTab::removeRegionButtonPressed);
    connect(ui.clearButton, &QPushButton::clicked, this, &FillerTab::clearButtonPressed);
}

void FillerTab::selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint)
{
    if (!newPoint)
        hidePointEditor();
    else
    {
        selectedPointWrapper.wrap(*newPoint);
        connect(&selectedPointWrapper, &PointWrapper::modified, canvas, &SmartCanvas::regionModified);

        showPointEditor();
    }
}

void FillerTab::specialPointChanged(int x, int y)
{
    ui.fillXInput->setValue(x);
    ui.fillYInput->setValue(y);
}

void FillerTab::constraintToggled(bool checked)
{
    canvas->setConstraintEnabled(checked);
}

void FillerTab::lineToggled(bool checked)
{
    canvas->setLineEnabled(checked);
}

void FillerTab::fillButtonPressed()
{
    core::BucFillRegionRenderer renderer;
    canvas->fillRegion(renderer, colorPicker->getColor());

    auto delta = renderer.getDuration().count();
    ui.fillTime->setText(QString(u8"Время заполнения: ") + QString::number(delta) + u8" мкс");
}

void FillerTab::fillStepButtonPressed()
{
    canvas->fillRegionWithStep(new core::AsyncBucFillRegionRenderer, colorPicker->getColor());
    ui.fillTime->setText(QString(u8"Время заполнения: --"));
}

void FillerTab::clearButtonPressed()
{
    canvas->clearCanvas();
    regionWrapper->clear();
}

void ui::FillerTab::removeRegionButtonPressed()
{
    canvas->deselect();
    region.clear();
    canvas->update();
}

void ui::FillerTab::clearOverlayButtonPressed()
{
    canvas->clearOverlay();
}

void FillerTab::showPointEditor()
{
    ui.pointEditorGroup->show();

    ui.xInput->disconnect();
    ui.yInput->disconnect();

    ui.xInput->setValue((*selectedPointWrapper).x);
    ui.yInput->setValue((*selectedPointWrapper).y);

    connect(ui.xInput, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, &selectedPointWrapper, &PointWrapper::setX);
    connect(ui.yInput, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, &selectedPointWrapper, &PointWrapper::setY);
}

void FillerTab::hidePointEditor()
{
    ui.pointEditorGroup->hide();
}
