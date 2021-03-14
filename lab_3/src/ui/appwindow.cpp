#include "appwindow.hpp"

ui::AppWindow::AppWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    segmentPage = new SegmentPage(this);
    spectrumPage = new SpectrumPage(this);
    statisticsPage = new StatisticsPage(this);

    ui.stackedWidget->addWidget(segmentPage);
    ui.stackedWidget->addWidget(spectrumPage);
    ui.stackedWidget->addWidget(statisticsPage);

    // init statistics page
    statisticsPage->initBars({ u8"ЦДА", u8"Брезенхэм действ.", u8"Библиотечный" });

    connect(ui.segmentButton, SIGNAL(clicked()), this, SLOT(selectSegmentPage()));
    connect(ui.spectrumButton, SIGNAL(clicked()), this, SLOT(selectSpectrumPage()));
    connect(ui.statisticsButton, SIGNAL(clicked()), this, SLOT(selectStatisticsPage()));
}

ui::AppWindow::~AppWindow()
{
    delete segmentPage;
    delete spectrumPage;
    delete statisticsPage;
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
    ui.stackedWidget->setCurrentWidget(statisticsPage);
}

void ui::AppWindow::updateTiming_BresenhemReal(qreal time)
{
    statisticsPage->setBarValue(1, time);
}

void ui::AppWindow::updateTiming_Library(qreal time)
{
    statisticsPage->setBarValue(2, time);
}
