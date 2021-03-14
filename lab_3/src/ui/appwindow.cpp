#include "appwindow.hpp"

#include "../core/bresrealsegren.hpp"
#include "../core/librarysegren.hpp"

using namespace core;

ui::AppWindow::AppWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // init renderers
    segmentRenderers.push_back(new BresRealSegRen());
    segmentRenderers.push_back(new LibSegRen());

    ui.setupUi(this);

    segmentPage = new SegmentPage(this);
    segmentPage->initAlogs(segmentRenderers);
    ui.stackedWidget->addWidget(segmentPage);

    spectrumPage = new SpectrumPage(this);
    spectrumPage->initAlgos(segmentRenderers);
    ui.stackedWidget->addWidget(spectrumPage);

    statisticsPage = new StatisticsPage(this);
    statisticsPage->initAlgos(segmentRenderers);
    ui.stackedWidget->addWidget(statisticsPage);

    connect(ui.segmentButton, SIGNAL(clicked()), this, SLOT(selectSegmentPage()));
    connect(ui.spectrumButton, SIGNAL(clicked()), this, SLOT(selectSpectrumPage()));
    connect(ui.statisticsButton, SIGNAL(clicked()), this, SLOT(selectStatisticsPage()));
}

ui::AppWindow::~AppWindow()
{
    delete segmentPage;
    delete spectrumPage;
    delete statisticsPage;

    for (const auto& renderer : segmentRenderers)
        delete renderer;
}

void ui::AppWindow::selectSegmentPage()
{
    ui.stackedWidget->setCurrentWidget(segmentPage);
}

void ui::AppWindow::selectSpectrumPage()
{
    ui.stackedWidget->setCurrentWidget(spectrumPage);
}

void ui::AppWindow::selectStatisticsPage()
{
    statisticsPage->updateBarValues();
    ui.stackedWidget->setCurrentWidget(statisticsPage);
}
