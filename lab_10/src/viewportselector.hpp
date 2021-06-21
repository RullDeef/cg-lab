#pragma once

#include <QWidget>
#include "ui_viewportselector.h"


class ViewportSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ViewportSelector(QWidget *parent = Q_NULLPTR);
    ~ViewportSelector() = default;

signals:
    void viewportChanged(double xStart, double xEnd, double zStart, double zEnd);

protected slots:
    void updateButtonPressed();
    void autoUpdateToggled(bool state);
    void inputChanged();

private:
    void emitChangedEvent();
    bool isAutoUpdate() const;

    Ui::ViewportSelector ui;
};
