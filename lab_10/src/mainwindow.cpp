#include <cmath>
#include <chrono>
#include <QTimer>
#include "mainwindow.hpp"
#include "viewportselector.hpp"
#include "orientationselector.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), canvas(new Canvas())
{
    ui.setupUi(this);

    ui.canvas->parentWidget()->layout()->replaceWidget(ui.canvas, canvas);

    FunctionSelector* functionSelector = new FunctionSelector(this, {
        {
            "0.2 * hypot(x, z) * sin(4 * atan(x/z))", [](double x, double z)
            {
                return 0.2 * std::hypot(x, z) * std::sin(4 * std::atan(x / z));
            }
        },
        {
            "pic1 + pic2 - 0.5 + 0.3 * z", [](double x, double z)
            {
                double pic1 = 0.6 / (1 + 60 * (x - 0.2) * (x - 0.2) + 20 * (z - 0.1) * (z - 0.1));
                double pic2 = 0.6 / (1 + 80 * (x + 0.4) * (x + 0.4) + 20 * (z + 0.1) * (z + 0.1));
                
                return pic1 + pic2;
            }
        },
        {
            "sin(hypot(x, z))", [](double x, double z)
            {
                return 0.2 * std::sin(10 * std::hypot(x, z));
            }
        }
    });
    connect(functionSelector, &FunctionSelector::functionSelected, this, &MainWindow::functionChanged);

    ui.functionCombo->parentWidget()->layout()->replaceWidget(ui.functionCombo, functionSelector);
    delete ui.functionCombo;
    ui.functionCombo = functionSelector;

    ViewportSelector* viewportSelector = new ViewportSelector(this);
    connect(viewportSelector, &ViewportSelector::viewportChanged, this, &MainWindow::viewportChanged);

    ui.viewportSelector->parentWidget()->layout()->replaceWidget(ui.viewportSelector, viewportSelector);
    delete ui.viewportSelector;
    ui.viewportSelector = nullptr;

    OrientationSelector* orientationSelector = new OrientationSelector(this);

    connect(orientationSelector, &OrientationSelector::orientationChanged, this, &MainWindow::orientationChanged);
    connect(orientationSelector, &OrientationSelector::orientationReset, this, &MainWindow::orientationReset);

    ui.orientationSelector->parentWidget()->layout()->replaceWidget(ui.orientationSelector, orientationSelector);
    delete ui.orientationSelector;
    ui.orientationSelector = nullptr;

    canvas->setSurface(functionSelector->getFunction());
}

void MainWindow::viewportChanged(double xStart, double xEnd, double zStart, double zEnd)
{
    canvas->setViewport(xStart, xEnd, -1.0, 1.0, zStart, zEnd);
    canvas->setCounts(20, 20, 4);

    redrawSurface();
}

void MainWindow::orientationChanged(double angleX, double angleY, double angleZ)
{
    canvas->rotateView(angleX, angleY, angleZ);

    redrawSurface();
}

void MainWindow::orientationReset()
{
    canvas->resetRotation();

    redrawSurface();
}

void MainWindow::redrawSurface()
{
    canvas->clear();
    canvas->drawSurface();
}

void MainWindow::functionChanged(FunctionSelector::Y_t func)
{
    canvas->setSurface(func);

    redrawSurface();
}
