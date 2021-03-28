#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include <vector>
#include <memory>
#include "ellipsepage.hpp"
#include "circlepage.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

public slots:
    void selectEllipsePage();
    void selectCirclePage();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<EllipsePage> ellipsePage;
    std::unique_ptr<CirclePage> circlePage;

    std::vector<std::shared_ptr<EllipseRenderer>> ellipseAlgorithms;
    std::vector<std::shared_ptr<CircleRenderer>> circleAlgorithms;
};
