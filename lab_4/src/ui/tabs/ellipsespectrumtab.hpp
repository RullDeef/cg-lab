#pragma once

#include <map>
#include "../core/primitiverenderer.hpp"
#include "canvastabwidget.hpp"

namespace ui
{
    class EllipseSpectrumTab : public CanvasTabWidget
    {
    public:
        EllipseSpectrumTab(core::RenderersContainer<core::Ellipse> renderers, QWidget* parent = Q_NULLPTR);

        void drawSpectrum();
        void drawOverlay();
        void clearCanvas();

    private:
        int ellipseA;
        int ellipseB;
        int radiusStep;

        QColor color = QColor(100, 255, 100);

        std::vector<core::PrimitiveRenderer<core::Ellipse>*> renderers;
        int rendererIndex = 0;
        int rendererOverlayIndex = 0;
    };
}
