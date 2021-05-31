#include <iostream>
#include <QMouseEvent>
#include <QColorDialog>
#include "ellipsetab.hpp"

using namespace ui;
using namespace core;

EllipseTab::EllipseTab(RenderersContainer<Ellipse> renderers, QWidget *parent)
    : CanvasTabWidget(u8"Ёллипсы", parent)
{
    addIntOption(u8"центр x", ellipseX, 0, 1400);
    addIntOption(u8"центр y", ellipseY, 0, 1400);
    addIntOption(u8"полуось a (OX)", ellipseA, 0, 2000);
    addIntOption(u8"полуось b (OY)", ellipseB, 0, 2000);

    QStringList names;
    for (const auto& renderer : renderers)
    {
        names.append(renderer.first);
        this->renderers.push_back(renderer.second);
    }

    addVariantOption(u8"алгоритм", rendererIndex, names);
    addColorOption(u8"цвет", color);

    addCustomAction(u8"построить", [this]() { drawEllipse(); });
    addCustomAction(u8"очистить", [this]() { clearCanvas(); });
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
