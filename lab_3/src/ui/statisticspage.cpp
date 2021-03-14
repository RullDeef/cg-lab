#include "statisticspage.hpp"
#include <QGraphicsView>
#include <QBarSet>

using namespace QtCharts;

ui::StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent),
    barSet(u8"Время выполнения алгоритма, мкс")
{
    ui.setupUi(this);
    initChart();

    // initBars({u8"ЦДА", u8"Брезенхэм действ.", u8"Библиотечный"});
    // setBarValue(0, 14.3);
    // setBarValue(1, 11.7);
    // setBarValue(2, 21.2);
}

void ui::StatisticsPage::initBars(const QStringList& strings)
{
    barSet.remove(0, catAxis.count());
    catAxis.clear();

    catAxis.append(strings);
    for (const auto& str : strings)
        barSet.append(0);

    valAxis.setRange(0, 10.0);
}

void ui::StatisticsPage::setBarValue(int index, qreal value)
{
    barSet.replace(index, value);

    // update range
    qreal maxTime = 0.0;
    for (int i = 0; i < catAxis.count(); i++)
        maxTime = std::max(maxTime, barSet[i]);
    valAxis.setRange(0, maxTime + 10.0);
}

void ui::StatisticsPage::initChart()
{
    series.append(&barSet);

    chart.setTitle(u8"Сравнение быстродействия");
    chart.addSeries(&series);

    // categories
    chart.addAxis(&catAxis, Qt::AlignBottom);
    series.attachAxis(&catAxis);

    // value ranges
    chart.addAxis(&valAxis, Qt::AlignLeft);
    series.attachAxis(&valAxis);

    chartView.setChart(&chart);
    chartView.setRenderHint(QPainter::Antialiasing);

    ui.horizontalLayout->insertWidget(0, &chartView);
    
}
