#include <cmath>
#include <chrono>
#include <QTimer>
#include "mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), canvas(new Canvas())
{
    ui.setupUi(this);

    ui.canvas->parentWidget()->layout()->replaceWidget(ui.canvas, canvas);

    connect(ui.resetRotation, &QPushButton::pressed, this, &MainWindow::redrawSurface);
    redrawSurface();
}

void MainWindow::redrawSurface()
{
    QTimer* timer = new QTimer();

    double startTime = double(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 10000;

    timer->callOnTimeout([&]() {
        canvas->clear();

        // canvas->drawSurface([](double x, double y, double z) {
        //     double pic1 = 0.6 / (1 + 60 * (x - 0.2) * (x - 0.2) + 20 * (z - 0.1) * (z - 0.1));
        //     double pic2 = 0.6 / (1 + 80 * (x + 0.4) * (x + 0.4) + 20 * (z + 0.1) * (z + 0.1));
        // 
        //     return pic1 + pic2 - 0.5 + 0.3 * z;
        // }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        double time = double(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 10000;
        time -= startTime;
        canvas->setViewport(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        canvas->setSurface([time](double x, double z) {
            double rx = x * std::cos(time) - z * std::sin(time);
            double rz = x * std::sin(time) + z * std::cos(time);

            return 0.2 * std::hypot(x, z) * std::sin(4 * std::atan(rx / rz));
        });

        canvas->setCounts(20, 20);
        canvas->rotateView(3.1415 / 6 * std::sin(5 * time), 3.1415 / 6 * time, 0);
        canvas->drawSurface();
    });

    timer->setInterval(60);
    timer->setSingleShot(false);
    timer->start();
}
