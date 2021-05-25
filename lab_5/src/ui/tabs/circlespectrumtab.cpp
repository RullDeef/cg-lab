#include <iostream>
#include <QMouseEvent>
#include <QColorDialog>
#include "circlespectrumtab.hpp"

using namespace ui;
using namespace core;

CircleSpectrumTab::CircleSpectrumTab(RenderersContainer<Circle> renderers, QWidget *parent)
    : CanvasTabWidget(u8"������ �����������", parent)
{
    addIntOption(u8"��� �������", circleRStep, 1, 1000);

    QStringList names;
    for (const auto& renderer : renderers)
    {
        names.append(renderer.first);
        this->renderers.push_back(renderer.second);
    }

    addVariantOption(u8"�������� ��������", rendererIndex, names);
    addVariantOption(u8"�������� ����������", rendererOverlayIndex, names);
    addColorOption(u8"���� ���������", color);

    addCustomAction(u8"��������� ��������", [this]() { drawSpectrum(); });
    addCustomAction(u8"��������� ����������", [this]() { drawOverlay(); });
    addCustomAction(u8"��������", [this]() { clearCanvas(); });
}

void CircleSpectrumTab::drawSpectrum()
{
    Circle circle;
    circle.x = canvasDrawer->boundingRect().center().x();
    circle.y = canvasDrawer->boundingRect().center().y();

    int maxR = std::min(canvasDrawer->boundingRect().width(), canvasDrawer->boundingRect().height()) / 2;

    for (circle.r = 0; circle.r < maxR; circle.r += circleRStep)
        renderers[rendererIndex]->draw(canvasDrawer->getImage(), circle, color);

    updateCanvas();
}

void CircleSpectrumTab::drawOverlay()
{
    Circle circle;
    circle.x = canvasDrawer->boundingRect().center().x();
    circle.y = canvasDrawer->boundingRect().center().y();

    int maxR = std::min(canvasDrawer->boundingRect().width(), canvasDrawer->boundingRect().height()) / 2;

    for (circle.r = 0; circle.r < maxR; circle.r += circleRStep)
        renderers[rendererOverlayIndex]->draw(canvasDrawer->getImage(), circle, Qt::white);

    updateCanvas();
}

void CircleSpectrumTab::clearCanvas()
{
    canvasDrawer->getImage().fill(Qt::white);
    updateCanvas();
}
