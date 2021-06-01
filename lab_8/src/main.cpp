#include <QtWidgets/QApplication>
#include "ui/mainwindow.hpp"
#include "ui/tabs/cuttertab.hpp"

#include "core/regionrenderer.hpp"

using namespace core;
using namespace ui;

void test_contours()
{
    BasicRegion region;

    auto p1 = region.appendPoint(0, 0);
    auto p2 = region.appendPoint(2, 0);
    auto p3 = region.appendPoint(3, 2);
    auto p4 = region.appendPoint(2, 3);
    auto p5 = region.appendPoint(0, 2);

    region.connectPoints(p3, p4);
    region.connectPoints(p1, p2);
    region.connectPoints(p5, p1);
    region.connectPoints(p2, p3);
    region.connectPoints(p4, p5);

    auto contour = region.getContours();
}

int main(int argc, char *argv[])
{
    // test_contours();
    // return 0;

    QApplication a(argc, argv);
    MainWindow w;

    CutterTab cuttertab;
    w.addInteractiveTab((InteractiveTabWidget*)&cuttertab);

    w.show();
    return a.exec();
}
