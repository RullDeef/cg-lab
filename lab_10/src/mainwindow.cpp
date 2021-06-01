#include <cmath>
#include <chrono>
#include "mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), canvas(new Canvas())
{
    ui.setupUi(this);

    ui.canvas->parentWidget()->layout()->replaceWidget(ui.canvas, canvas);

    connect(ui.resetRotation, &QPushButton::pressed, this, &MainWindow::redrawSurface);
}

void MainWindow::redrawSurface()
{
    canvas->clear();

    // canvas->drawSurface([](double x, double y, double z) {
    //     double pic1 = 0.6 / (1 + 60 * (x - 0.2) * (x - 0.2) + 20 * (z - 0.1) * (z - 0.1));
    //     double pic2 = 0.6 / (1 + 80 * (x + 0.4) * (x + 0.4) + 20 * (z + 0.1) * (z + 0.1));
    // 
    //     return pic1 + pic2 - 0.5 + 0.3 * z;
    // }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    double time = double(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 30000;
    canvas->drawSurface([time](double x, double y, double z) {
        double rx = x * std::cos(time) - z * std::sin(time);
        double rz = x * std::sin(time) + z * std::cos(time);

        return 0.2 * std::hypot(x, z) * std::sin(16 * std::atan(rx / rz)) + 0.3 * z;
    }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}
