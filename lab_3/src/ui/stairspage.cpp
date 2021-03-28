
#include "stairspage.hpp"

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
            line->append(i, calcStairsCount(300, i));

        chart.addSeries(line);
        line->attachAxis(&lenAxis);
        line->attachAxis(&angleAxis);
    }
}

int ui::StairsPage::calcStairsCount(double segLength, int angleDeg)
{
    constexpr auto degToRad = 3.14159265358979323846 / 180;

    if (angleDeg > 45)
        angleDeg = 90 - angleDeg;

    return std::round(segLength * std::sin(angleDeg * degToRad));
}

void ui::StairsPage::initChart()
{
    QFont font = lenAxis.titleFont();
    font.setPointSize(14);

    lenAxis.setTitleText(u8"Количество ступенек");
    lenAxis.setTitleFont(font);
    angleAxis.setTitleText(u8"Угол наклона отрезка");
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
