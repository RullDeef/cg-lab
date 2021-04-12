#include <iostream>
#include <QMouseEvent>
#include <QColorDialog>
#include "ellipsetab.hpp"

using namespace ui;
using namespace core;

EllipseTab::EllipseTab(RenderersContainer<Ellipse> renderers, QWidget *parent)
    : CanvasTabWidget(u8"�������", parent)
{
    addIntOption(u8"����� x", ellipseX, 0, 1400);
    addIntOption(u8"����� y", ellipseY, 0, 1400);
    addIntOption(u8"������� a (OX)", ellipseA, 0, 2000);
    addIntOption(u8"������� b (OY)", ellipseB, 0, 2000);

    QStringList names;
    for (const auto& renderer : renderers)
    {
        names.append(renderer.first);
        this->renderers.push_back(renderer.second);
    }

    addVariantOption(u8"��������", rendererIndex, names);
    addColorOption(u8"����", color);

    addCustomAction(u8"���������", [this]() { drawEllipse(); });
    addCustomAction(u8"��������", [this]() { clearCanvas(); });
}

void EllipseTab::drawEllipse()
{
    Ellipse ellipse;
    ellipse.x = ellipseX;
    ellipse.y = ellipseY;
    ellipse.a = ellipseA;
    ellipse.b = ellipseB;

    renderers[rendererIndex]->draw(canvasDrawer->getImage(), ellipse, color);
    updateCanvas();
}

void EllipseTab::clearCanvas()
{
    canvasDrawer->getImage().fill(Qt::white);
    updateCanvas();
}
