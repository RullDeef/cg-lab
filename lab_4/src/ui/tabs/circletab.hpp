#pragma once

#include <map>
#include "../core/primitiverenderer.hpp"
#include "canvastabwidget.hpp"

namespace ui
{
    class CircleTab : public CanvasTabWidget
    {
    public:
        CircleTab(std::map<QString, core::PrimitiveRenderer<core::Circle>*> renderers, QWidget* parent = Q_NULLPTR);

        void drawCircle();

    private:
        int circleX;
        int circleY;
        int circleR;

        QColor color = QColor(100, 255, 100);

        std::vector<core::PrimitiveRenderer<core::Circle>*> renderers;
        int rendererIndex = 0;
    };
}
