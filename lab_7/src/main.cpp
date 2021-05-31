#include <QtWidgets/QApplication>
#include "ui/mainwindow.hpp"
#include "ui/tabs/fillertab.hpp"
#include "ui/tabs/cuttertab.hpp"
#include "core/fill/boundregionrenderer.hpp"

using namespace core;
using namespace ui;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    BoundRegionRenderer boundregren;

    FillerTab fillertab(&boundregren);
    CutterTab cuttertab;

    w.addInteractiveTab((InteractiveTabWidget*)&fillertab);
    w.addInteractiveTab((InteractiveTabWidget*)&cuttertab);

    w.show();
    return a.exec();
}
