#include <iostream>
#include <QMouseEvent>
#include <QColorDialog>
#include "ellipsespectrumtab.hpp"

using namespace ui;
using namespace core;

EllipseSpectrumTab::EllipseSpectrumTab(RenderersContainer<Ellipse> renderers, QWidget *parent)
    : CanvasTabWidget(u8"������ ��������", parent)
{
    addIntOption(u8"��������� ������� a (OX)", ellipseA, 1, 1000);
    addIntOption(u8"��������� ������� b (OY)", ellipseB, 1, 1000);
    addIntOption(u8"��� ��������", radiusStep, 1, 1000);

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

void EllipseSpectrumTab::drawSpectrum()
{
    Ellipse ellipse;
    ellipse.x = std::round(canvasDrawer->boundingRect().center().x());
    ellipse.y = std::round(canvasDrawer->boundingRect().center().y());

    int maxR = std::min(canvasDrawer->boundingRect().width(), canvasDrawer->boundingRect().height()) / 2;

    for (ellipse.a = ellipseA, ellipse.b = ellipseB; ellipse.a < maxR && ellipse.b < maxR; ellipse.a += radiusStep, ellipse.b += radiusStep)
        renderers[rendererIndex]->draw(canvasDrawer->getImage(), ellipse, color);

    updateCanvas();
}

void EllipseSpectrumTab::drawOverlay()
{
    Ellipse ellipse;
    ellipse.x = std::round(canvasDrawer->boundingRect().center().x());
    ellipse.y = std::round(canvasDrawer->boundingRect().center().y());

    int maxR = std::min(canvasDrawer->boundingRect().width(), canvasDrawer->boundingRect().height()) / 2;

    for (ellipse.a = ellipseA, ellipse.b = ellipseB; ellipse.a < maxR && ellipse.b < maxR; ellipse.a += radiusStep, ellipse.b += radiusStep)
        renderers[rendererOverlayIndex]->draw(canvasDrawer->getImage(), ellipse, Qt::white);

    updateCanvas();
}

void EllipseSpectrumTab::clearCanvas()
{
    canvasDrawer->getImage().fill(Qt::white);
    updateCanvas();
}
