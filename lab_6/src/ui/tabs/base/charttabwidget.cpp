#include "charttabwidget.hpp"
#include <QGraphicsView>
#include <QBarSet>

using namespace QtCharts;

ui::ChartTabWidget::ChartTabWidget(const char *name, const char* chartTitle, QWidget *parent)
    : InteractiveTabWidget(name, parent)
{
    ui.setupUi(this);
    initChart(chartTitle);
}

void ui::ChartTabWidget::initChart(const char* title)
{
    QFont font = chart.font();
    font.setPointSize(14);

    chart.setTitleFont(font);
    chart.setTitle(title);

    chartView.setChart(&chart);
    chartView.setRenderHint(QPainter::Antialiasing);

    ui.horizontalLayout->insertWidget(0, &chartView);
}

ui::LineChartTabWidget::LineChartTabWidget(const char* name, const char* chartTitle, QWidget* parent)
    : ChartTabWidget(name, chartTitle, parent)
{
    initChart();
}

ui::LineChartTabWidget::~LineChartTabWidget()
{
    for (const auto& iter : series)
        delete iter.second;
}

void ui::LineChartTabWidget::addPoint(const QString& seriesName, qreal x, qreal y)
{
    if (series.count(seriesName) == 0)
    {
        auto ser = new QtCharts::QLineSeries();
        series.insert({ seriesName, ser });
        ser->setName(seriesName);

        getChart().addSeries(ser);
        getChart().setAxisX(&xAxis, ser);
        getChart().setAxisY(&yAxis, ser);
        ser->attachAxis(&xAxis);
        ser->attachAxis(&yAxis);
    }

    series[seriesName]->append(x, y);
    xAxis.setMin(x < xAxis.min() ? x : xAxis.min());
    yAxis.setMin(y < yAxis.min() ? y : yAxis.min());
    xAxis.setMax(x > xAxis.max() ? x : xAxis.max());
    yAxis.setMax(y > yAxis.max() ? y : yAxis.max());
}

void ui::LineChartTabWidget::refreshSeries()
{
}

void ui::LineChartTabWidget::clearSeries()
{
    for (auto& iter : series)
        iter.second->clear();

    xAxis.setMin(0);
    xAxis.setMax(0);
    yAxis.setMin(0);
    yAxis.setMax(0);
}

void ui::LineChartTabWidget::initChart()
{
    
}
