#pragma once

#include "../core/primitiverenderer.hpp"
#include "canvastabwidget.hpp"

namespace ui
{
    class EllipseTab : public CanvasTabWidget
    {
    public:
        EllipseTab(core::RenderersContainer<core::Ellipse> renderers, QWidget* parent = Q_NULLPTR);

        void drawEllipse();
        void clearCanvas();

    private:
        int ellipseX;
        int ellipseY;
        int ellipseA;
        int ellipseB;

        QColor color = QColor(100, 255, 100);

        std::vector<core::PrimitiveRenderer<core::Ellipse>*> renderers;
        int rendererIndex = 0;
    };
}
