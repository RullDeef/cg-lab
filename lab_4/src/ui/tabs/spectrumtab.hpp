#pragma once

#include <map>
#include "../core/primitiverenderer.hpp"
#include "canvastabwidget.hpp"

namespace ui
{
    class SpectrumTab : public CanvasTabWidget
    {
    public:
        SpectrumTab(std::map<QString, core::PrimitiveRenderer<core::Circle>*> renderers, QWidget* parent = Q_NULLPTR);

        void drawSpectrum();
        void drawOverlay();
        void clearCanvas();

    private:
        int circleRStep;

        QColor color = QColor(100, 255, 100);

        std::vector<core::PrimitiveRenderer<core::Circle>*> renderers;
        int rendererIndex = 0;
        int rendererOverlayIndex = 0;
    };
}
