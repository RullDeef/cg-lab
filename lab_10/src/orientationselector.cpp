#include "orientationselector.hpp"


OrientationSelector::OrientationSelector(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.rxpdaButton, &QPushButton::pressed, this, &OrientationSelector::rxpdaButtonPressed);
    connect(ui.rxmdaButton, &QPushButton::pressed, this, &OrientationSelector::rxmdaButtonPressed);
    connect(ui.rypdaButton, &QPushButton::pressed, this, &OrientationSelector::rypdaButtonPressed);
    connect(ui.rymdaButton, &QPushButton::pressed, this, &OrientationSelector::rymdaButtonPressed);
    connect(ui.resetRotation, &QPushButton::pressed, this, &OrientationSelector::resetButtonPressed);
}

void OrientationSelector::rxpdaButtonPressed()
{
    emit orientationChanged(getAngleDelta(), 0.0, 0.0);
}

void OrientationSelector::rxmdaButtonPressed()
{
    emit orientationChanged(-getAngleDelta(), 0.0, 0.0);
}

void OrientationSelector::rypdaButtonPressed()
{
    emit orientationChanged(0.0, getAngleDelta(), 0.0);
}

void OrientationSelector::rymdaButtonPressed()
{
    emit orientationChanged(0.0, -getAngleDelta(), 0.0);
}

void OrientationSelector::resetButtonPressed()
{
    emit orientationReset();
}

double OrientationSelector::getAngleDelta() const
{
    return qDegreesToRadians(ui.daInput->value());
}
