#include <map>

#include "mainwindow.hpp"
#include <QtWidgets/QApplication>

// circle renderers
#include "core/libcirren.hpp"
#include "core/paramcirren.hpp"
#include "core/brescirren.hpp"
#include "core/mediancirren.hpp"
#include "core/canoncirren.hpp"

// ellipse renderers
#include "core/libellren.hpp"
#include "core/paramellren.hpp"
#include "core/bresellren.hpp"
#include "core/canonellren.hpp"
#include "core/medianellren.hpp"

#include "ui/tabs/circletab.hpp"
#include "ui/tabs/ellipsetab.hpp"
#include "ui/tabs/circlespectrumtab.hpp"
#include "ui/tabs/ellipsespectrumtab.hpp"
#include "ui/tabs/statisticstab.hpp"
#include "ui/tabs/dynamicstatisticstab.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    core::LibraryCircleRenderer libcirren;
    core::ParametricCircleRenderer paramcirren;
    core::BresenhemCircleRenderer brescirren;
    core::MedianCircleRenderer mediancirren;
    core::CanonicCircleRenderer canoncirren;

    core::LibraryEllipseRenderer libellren;
    core::ParametricEllipseRenderer paramellren;
    core::BresenhemEllipseRenderer bresellren;
    core::CanonicEllipseRenderer canonellren;
    core::MedianEllipseRenderer medianellren;

    core::RenderersContainer<core::Circle> circleRenderers = {
        { libcirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&libcirren },
        { paramcirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&paramcirren },
        { brescirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&brescirren },
        { mediancirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&mediancirren },
        { canoncirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&canoncirren }
    };

    core::RenderersContainer<core::Ellipse> ellipseRenderers = {
        { libellren.getName(), (core::PrimitiveRenderer<core::Ellipse>*)&libellren },
        { paramellren.getName(), (core::PrimitiveRenderer<core::Ellipse>*)&paramellren },
        { bresellren.getName(), (core::PrimitiveRenderer<core::Ellipse>*)&bresellren },
        { canonellren.getName(), (core::PrimitiveRenderer<core::Ellipse>*)&canonellren },
        { medianellren.getName(), (core::PrimitiveRenderer<core::Ellipse>*)&medianellren }
    };

    ui::CircleTab circleTab(circleRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&circleTab);

    ui::EllipseTab ellipseTab(ellipseRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&ellipseTab);

    ui::CircleSpectrumTab circleSpectrumTab(circleRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&circleSpectrumTab);
    
    ui::EllipseSpectrumTab ellipseSpectrumTab(ellipseRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&ellipseSpectrumTab);

    ui::StatisticsTab statisticsTab(circleRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&statisticsTab);

    ui::DynamicStatisticsTab dynamicStatisticsTab(ellipseRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&dynamicStatisticsTab);

    w.show();
    return a.exec();
}
