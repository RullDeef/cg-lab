#include "mainwindow.hpp"
#include <QtWidgets/QApplication>

#include "ui/tabs/canvastabwidget.hpp"
#include "ui/tabs/circletab.hpp"

#include <map>
#include "core/libcirren.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    core::LibraryCircleRenderer libcirren;

    ui::CircleTab tab({
        { libcirren.getName(), (core::PrimitiveRenderer<core::Circle>*)&libcirren }
    }, &w);
    w.addInteractiveTab((ui::InteractiveTabWidget*)&tab);

    w.show();
    int result = a.exec();



    return result;
}
