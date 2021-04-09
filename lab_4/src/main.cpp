#include <map>

#include "mainwindow.hpp"
#include <QtWidgets/QApplication>

#include "ui/tabs/canvastabwidget.hpp"
#include "ui/tabs/circletab.hpp"
#include "ui/tabs/spectrumtab.hpp"

#include "core/libcirren.hpp"
#include "core/paramcirren.hpp"
#include "core/brescirren.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    core::LibraryCircleRenderer libcirren;
    core::ParametricCircleRenderer paramcirren;
    core::BresenhemCircleRenderer brescirren;

    std::map<QString, core::PrimitiveRenderer<core::Circle>*> circleRenderers = {
        { libcirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&libcirren },
        { paramcirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&paramcirren },
        { brescirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&brescirren }
    };

    ui::CircleTab circleTab(circleRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&circleTab);

    ui::SpectrumTab spectrumTab(circleRenderers, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&spectrumTab);

    w.show();
    return a.exec();
}
