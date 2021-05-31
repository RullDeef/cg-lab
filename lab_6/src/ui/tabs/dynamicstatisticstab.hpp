#pragma once

#include "../core/primitiverenderer.hpp"
#include "charttabwidget.hpp"

namespace ui
{
    class DynamicStatisticsTab : public LineChartTabWidget, public WidgetOptionsStrategy
    {
    public:
        DynamicStatisticsTab(core::RenderersContainer<core::Ellipse> renderers, QWidget* parent = Q_NULLPTR);
        virtual ~DynamicStatisticsTab() {}

    protected:
        void updateGraph(double ellipseA);

    private:
        core::RenderersContainer<core::Ellipse> renderers;

        int ellipseA;
    };
}
