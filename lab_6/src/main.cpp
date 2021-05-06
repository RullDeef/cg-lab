#include <QtWidgets/QApplication>
#include "ui/mainwindow.hpp"
#include "ui/tabs/fillertab.hpp"
#include "core/fill/boundregionrenderer.hpp"

using namespace core;
using namespace ui;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    BoundRegionRenderer boundregren;
    FillerTab fillertab(&boundregren);

    w.addInteractiveTab((InteractiveTabWidget*)&fillertab);

    w.show();
    return a.exec();
}
