#include <QtWidgets/QApplication>
#include "ui/mainwindow.hpp"
#include "ui/tabs/cuttertab.hpp"

using namespace core;
using namespace ui;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    CutterTab cuttertab;

    w.addInteractiveTab((InteractiveTabWidget*)&cuttertab);

    w.show();
    return a.exec();
}
