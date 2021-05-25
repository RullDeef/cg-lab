#include "appwindow.hpp"

#include "../core/ddasegren.hpp"
#include "../core/bresrealsegren.hpp"
#include "../core/bresintsegren.hpp"
#include "../core/bressmoothsegren.hpp"
#include "../core/librarysegren.hpp"
#include "../core/wusegren.hpp"

using namespace core;

ui::AppWindow::AppWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // init renderers
    segmentRenderers.push_back(new DDASegRen());
    segmentRenderers.push_back(new BresRealSegRen());
    segmentRenderers.push_back(new BresIntSegRen());
    segmentRenderers.push_back(new BresSmoothSegRen());
    segmentRenderers.push_back(new LibSegRen());
    segmentRenderers.push_back(new WuSegRen());

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

    stairsPage = new StairsPage(this);
    stairsPage->InitAlgos(segmentRenderers);
    ui.stackedWidget->addWidget(stairsPage);

    connect(ui.segmentButton, SIGNAL(clicked()), this, SLOT(selectSegmentPage()));
    connect(ui.spectrumButton, SIGNAL(clicked()), this, SLOT(selectSpectrumPage()));
    connect(ui.statisticsButton, SIGNAL(clicked()), this, SLOT(selectStatisticsPage()));
    connect(ui.stairsButton, SIGNAL(clicked()), this, SLOT(selectStairsPage()));
}

ui::AppWindow::~AppWindow()
{
    delete segmentPage;
    delete spectrumPage;
    delete statisticsPage;
    delete stairsPage;

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

void ui::AppWindow::selectStairsPage()
{
    ui.stackedWidget->setCurrentWidget(stairsPage);
}
