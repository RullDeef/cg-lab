#pragma once

#include <QtWidgets/QMainWindow>
#include "canvas/canvas.hpp"
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

protected slots:
    void redrawSurface();

private:
    Ui::MainWindowClass ui;
    Canvas* canvas;
};
