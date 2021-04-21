#pragma once

#include "../core/primitiverenderer.hpp"
#include "charttabwidget.hpp"

namespace ui
{
    class StatisticsTab : public LineChartTabWidget
    {
    public:
        StatisticsTab(core::RenderersContainer<core::Circle> renderers, QWidget* parent = Q_NULLPTR);
    };
}
