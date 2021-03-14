#include "ui/appwindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ui::AppWindow window;
    window.showMaximized();
    return app.exec();
}
