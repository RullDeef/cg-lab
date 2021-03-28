#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    algorithms.push_back(std::make_shared<LibEllRen>());

    ui.setupUi(this);

    ellipsePage = std::make_unique<EllipsePage>(this);
    ellipsePage->initAlgos(algorithms);
    ui.stackedWidget->addWidget(&*ellipsePage);

    connect(ui.ellipseButton, SIGNAL(clicked()), this, SLOT(selectEllipsePage()));
}

void MainWindow::selectEllipsePage()
{
    ui.stackedWidget->setCurrentWidget(&*ellipsePage);
}
