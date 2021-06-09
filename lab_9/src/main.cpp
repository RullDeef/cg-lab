#include <QtWidgets/QApplication>
#include "ui/mainwindow.hpp"
#include "ui/tabs/cuttertab.hpp"

#include "core/regionrenderer.hpp"

using namespace core;
using namespace ui;

void test_contours()
{
    BasicRegion region;

    Point& p1 = region.appendPoint(10, 10);
    Point& p2 = region.appendPoint(50, 10);
    Point& p3 = region.appendPoint(30, 50);

    region.connectPoints(p1, p2);
    region.connectPoints(p2, p3);
    region.connectPoints(p3, p1);

    Point& p4 = region.appendPoint(30, 30);
    Point& p5 = region.appendPoint(40, 20);
    Point& p6 = region.appendPoint(20, 20);

    region.connectPoints(p4, p5);
    region.connectPoints(p5, p6);
    region.connectPoints(p6, p4);

    auto contour1 = region.getContourWith(p3);
    auto contour2 = region.getContourWith(p5);
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
