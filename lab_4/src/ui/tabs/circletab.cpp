#include <iostream>
#include <QMouseEvent>
#include <QColorDialog>
#include "circletab.hpp"

using namespace ui;
using namespace core;

CircleTab::CircleTab(RenderersContainer<Circle> renderers, QWidget *parent)
    : CanvasTabWidget(u8"??????????", parent)
{
    addIntOption(u8"????? x", circleX, 0, 1400);
    addIntOption(u8"????? y", circleY, 0, 1400);
    addIntOption(u8"??????", circleR, 0, 2000);

    QStringList names;
    for (const auto& renderer : renderers)
    {
        names.append(renderer.first);
        this->renderers.push_back(renderer.second);
    }

    addVariantOption(u8"????????", rendererIndex, names);
    addColorOption(u8"????", color);

    addCustomAction(u8"?????????", [this]() { drawCircle(); });
    addCustomAction(u8"????????", [this]() { clearCanvas(); });
}

void CircleTab::drawCircle()
{
    Circle circle;
    circle.x = circleX;
    circle.y = circleY;
    circle.r = circleR;
    
    renderers[rendererIndex]->draw(canvasDrawer->getImage(), circle, color);
    updateCanvas();
}

void CircleTab::clearCanvas()
{
    canvasDrawer->getImage().fill(Qt::white);
    updateCanvas();
}
