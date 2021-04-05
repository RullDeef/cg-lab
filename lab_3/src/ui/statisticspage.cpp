#include "statisticspage.hpp"
#include <QGraphicsView>
#include <QBarSet>

using namespace QtCharts;

ui::StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent),
    barSet(u8"Время выполнения алгоритма (мкс/пикс.)")
{
    ui.setupUi(this);
    initChart();
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

    updateRange();

    useAlgosOnce();
    updateBarValues();
}

void ui::StatisticsPage::useAlgosOnce()
{
    QImage image(1080, 1080, QImage::Format::Format_ARGB32);

    constexpr auto tau = qDegreesToRadians(360.0);
    constexpr auto angleStep = tau / 400;

    core::Segment segment;
    segment.color = QColor(Qt::red);
    segment.x1 = 540;
    segment.y1 = 540;

    for (float angle = 0; angle < tau; angle += angleStep)
    {
        segment.x2 = segment.x1 + 500 * cos(angle);
        segment.y2 = segment.y1 + 500 * sin(angle);

        for (const auto& algo : segmentRenderers)
            algo->drawSegment(image, segment);
    }

    for (const auto& algo : segmentRenderers)
        algo->stopTiming();
}

void ui::StatisticsPage::updateBarValues()
{
    int i = 0;
    for (const auto& renderer : segmentRenderers)
        barSet.replace(i++, renderer->getMeanDrawTime());
    updateRange();
}

void ui::StatisticsPage::setBarValue(int index, qreal value)
{
    barSet.replace(index, value);
    updateRange();
}

void ui::StatisticsPage::updateRange()
{
    qreal maxTime = 0.01;
    for (int i = 0; i < catAxis.count(); i++)
        maxTime = std::max(maxTime, barSet[i]);
    valAxis.setRange(0, maxTime / 0.85);
}

void ui::StatisticsPage::initChart()
{
    series.append(&barSet);

    QFont font = chart.font();
    font.setPointSize(14);
    chart.setTitleFont(font);
    chart.setTitle(u8"Сравнение быстродействия");
    chart.addSeries(&series);

    // categories
    chart.addAxis(&catAxis, Qt::AlignBottom);
    font.setPointSize(12);
    catAxis.setLabelsFont(font);
    series.attachAxis(&catAxis);

    // value ranges
    chart.addAxis(&valAxis, Qt::AlignLeft);
    series.attachAxis(&valAxis);

    chartView.setChart(&chart);
    chartView.setRenderHint(QPainter::Antialiasing);

    ui.horizontalLayout->insertWidget(0, &chartView);
}

