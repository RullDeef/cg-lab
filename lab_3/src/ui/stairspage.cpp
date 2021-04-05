
#include "stairspage.hpp"
#include "../core/stairscounter.hpp"

ui::StairsPage::StairsPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    initChart();
}

ui::StairsPage::~StairsPage()
{
    for (const auto& line : lineSeries)
        delete line;
}

void ui::StairsPage::InitAlgos(const std::list<core::SegmentRenderer*>& algos)
{
    segmentRenderers.clear();
    std::copy(algos.begin(), algos.end(), std::back_inserter(segmentRenderers));

    lineSeries.clear();
    lineSeries.reserve(segmentRenderers.size());
    for (const auto& algo : segmentRenderers)
    {
        QtCharts::QLineSeries* line = new QtCharts::QLineSeries();
        lineSeries.push_back(line);

        line->setName(algo->getName());

        for (int i = 0; i <= 90; i++)
        {
            core::Segment segment;
            segment.color = Qt::black;
            segment.x1 = 0.0;
            segment.y1 = 0.0;

            double angle = qDegreesToRadians(double(i));
            segment.x2 = 300.0 * std::cos(angle);
            segment.y2 = 300.0 * std::sin(angle);

            line->append(i, core::StairsCounter::countStairsAmountForSegment(algo, segment));
        }

        chart.addSeries(line);
        line->attachAxis(&lenAxis);
        line->attachAxis(&angleAxis);
    }
}

void ui::StairsPage::initChart()
{
    QFont font = lenAxis.titleFont();
    font.setPointSize(14);

    lenAxis.setTitleText(u8"Количество ступенек");
    lenAxis.setTitleFont(font);
    angleAxis.setTitleText(u8"Угол наклона отрезка (градусы)");
    angleAxis.setTitleFont(font);

    chart.setTitle(u8"Анализ ступенчатости");
    chart.setTitleFont(font);

    lenAxis.setTickAnchor(0);
    lenAxis.setTickInterval(10.0);
    lenAxis.setTickType(QValueAxis::TicksDynamic);

    angleAxis.setTickAnchor(5.0);
    angleAxis.setTickInterval(5.0);
    angleAxis.setTickType(QValueAxis::TicksDynamic);

    chart.addAxis(&lenAxis, Qt::AlignLeft);
    chart.addAxis(&angleAxis, Qt::AlignBottom);

    chartView.setChart(&chart);
    chartView.setRenderHint(QPainter::Antialiasing);

    ui.horizontalLayout->insertWidget(0, &chartView);
}
