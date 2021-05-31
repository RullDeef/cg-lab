#include <QtWidgets/QApplication>
#include "ui/mainwindow.hpp"
#include "ui/tabs/fillertab.hpp"
#include "core/fill/boundregionrenderer.hpp"
#include "core/fill/bucfillregionrenderer.hpp"

using namespace core;
using namespace ui;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    BucFillRegionRenderer renderer;
    FillerTab fillertab(&renderer);

    w.addInteractiveTab((InteractiveTabWidget*)&fillertab);

    w.show();
    return a.exec();
}
