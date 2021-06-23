#pragma once

#include <QWidget>
#include "ui_orientationselector.h"


class OrientationSelector : public QWidget
{
    Q_OBJECT

public:
    OrientationSelector(QWidget *parent = Q_NULLPTR);

signals:
    void orientationChanged(double angleX, double angleY, double angleZ);
    void orientationReset();

protected:
    void rxpdaButtonPressed();
    void rxmdaButtonPressed();
    void rypdaButtonPressed();
    void rymdaButtonPressed();
    void resetButtonPressed();

private:
    double getAngleDelta() const;

    Ui::OrientationSelector ui;
};
