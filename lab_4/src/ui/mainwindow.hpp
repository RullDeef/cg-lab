#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include <vector>
#include <memory>
#include "ellipsepage.hpp"
#include "../core/libellren.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

public slots:
    void selectEllipsePage();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<EllipsePage> ellipsePage;

    std::vector<std::shared_ptr<EllipseRenderer>> algorithms;
};
