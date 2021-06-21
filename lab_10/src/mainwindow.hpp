#pragma once

#include <QtWidgets/QMainWindow>
#include "canvas/canvas.hpp"
#include "functionselector.hpp"
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

protected slots:
    void functionChanged(FunctionSelector::Y_t func);
    void viewportChanged(double xStart, double xEnd, double zStart, double zEnd);
    void orientationChanged(double angleX, double angleY, double angleZ);
    void orientationReset();

protected:
    void redrawSurface();

private:
    Ui::MainWindowClass ui;
    Canvas* canvas;
};
