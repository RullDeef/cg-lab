#include "viewportselector.hpp"

ViewportSelector::ViewportSelector(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.xStart, &QDoubleSpinBox::valueChanged, this, &ViewportSelector::inputChanged);
    connect(ui.xEnd, &QDoubleSpinBox::valueChanged, this, &ViewportSelector::inputChanged);
    connect(ui.zStart, &QDoubleSpinBox::valueChanged, this, &ViewportSelector::inputChanged);
    connect(ui.zEnd, &QDoubleSpinBox::valueChanged, this, &ViewportSelector::inputChanged);

    connect(ui.autoUpdateCheckBox, &QCheckBox::toggled, this, &ViewportSelector::autoUpdateToggled);
    connect(ui.updateButton, &QPushButton::pressed, this, &ViewportSelector::updateButtonPressed);
}

void ViewportSelector::updateButtonPressed()
{
    emitChangedEvent();
}

void ViewportSelector::autoUpdateToggled(bool state)
{
    ui.updateButton->setDisabled(state);
}

void ViewportSelector::inputChanged()
{
    if (isAutoUpdate())
        emitChangedEvent();
}

void ViewportSelector::emitChangedEvent()
{
    double xStart = ui.xStart->value();
    double xEnd = ui.xEnd->value();
    double zStart = ui.zStart->value();
    double zEnd = ui.zEnd->value();

    emit viewportChanged(xStart, xEnd, zStart, zEnd);
}

bool ViewportSelector::isAutoUpdate() const
{
    return ui.autoUpdateCheckBox->checkState() == Qt::CheckState::Checked;
}
