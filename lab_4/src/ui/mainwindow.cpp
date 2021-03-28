#include "mainwindow.hpp"

#include "../core/libellren.hpp"
#include "../core/libcirren.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ellipseAlgorithms.push_back(std::make_shared<LibEllRen>());
    circleAlgorithms.push_back(std::make_shared<LibCirRen>());

    ui.setupUi(this);

    circlePage = std::make_unique<CirclePage>(this);
    circlePage->initAlgos(circleAlgorithms);
    ui.stackedWidget->addWidget(&*circlePage);

    ellipsePage = std::make_unique<EllipsePage>(this);
    ellipsePage->initAlgos(ellipseAlgorithms);
    ui.stackedWidget->addWidget(&*ellipsePage);

    connect(ui.ellipseButton, SIGNAL(clicked()), this, SLOT(selectEllipsePage()));
    connect(ui.circleButton, SIGNAL(clicked()), this, SLOT(selectCirclePage()));
}

void MainWindow::selectEllipsePage()
{
    ui.stackedWidget->setCurrentWidget(&*ellipsePage);
}

void MainWindow::selectCirclePage()
{
    ui.stackedWidget->setCurrentWidget(&*circlePage);
}
