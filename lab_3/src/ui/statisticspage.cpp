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

    connect(ui.updateButton, SIGNAL(clicked()), this, SLOT(updateBarValues()));
}

void ui::StatisticsPage::initAlgos(const std::list<core::SegmentRenderer*>& algos)
{
    segmentRenderers = algos;

    barSet.remove(0, catAxis.count());
    catAxis.clear();

    QStringList strings;
    std::transform(algos.begin(), algos.end(), std::back_inserter(strings),
        [](const core::SegmentRenderer* renderer) -> QString { return renderer->getName(); });
    catAxis.append(strings);

    for (const auto& algo : algos)
        barSet.append(0);

    valAxis.setRange(0, 2.0);
}

void ui::StatisticsPage::updateBarValues()
{
    int i = 0;
    for (const auto& renderer : segmentRenderers)
        barSet.replace(i++, renderer->getMeanDrawTime().count());
    updateRange();
}

void ui::StatisticsPage::setBarValue(int index, qreal value)
{
    barSet.replace(index, value);
    updateRange();
}

void ui::StatisticsPage::updateRange()
{
    qreal maxTime = 0.0;
    for (int i = 0; i < catAxis.count(); i++)
        maxTime = std::max(maxTime, barSet[i]);
    valAxis.setRange(0, maxTime + 2.0);
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

