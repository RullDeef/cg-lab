#include <QMouseEvent>
#include <QColorDialog>
#include "circletab.hpp"

#include <iostream>

using namespace ui;
using namespace core;

CircleTab::CircleTab(std::map<QString, PrimitiveRenderer<Circle>*> renderers, QWidget *parent)
    : CanvasTabWidget(u8"����������", parent)
{
    addIntOption(u8"����� x", circleX);
    addIntOption(u8"����� y", circleY);
    addIntOption(u8"������", circleR);

    QStringList names;
    for (const auto& renderer : renderers)
    {
        names.append(renderer.first);
        this->renderers.push_back(renderer.second);
    }

    addVariantOption(u8"��������", rendererIndex, names);
    addColorOption(u8"����", color);

    addCustomAction(u8"���������", [this]() { drawCircle(); });
    addCustomAction(u8"��������", [this]() { clearCanvas(); });
}

void CircleTab::drawCircle()
{
    Circle circle;
    circle.x = circleX;
    circle.y = circleY;
    circle.r = circleR;
    
    renderers[rendererIndex]->drawPrimitive(&pixmap, circle, color);

    updateCanvas();
}
