
#include "stairspage.hpp"

ui::StairsPage::StairsPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    initChart();
}

ui::StairsPage::~StairsPage()
{
}

void ui::StairsPage::initChart()
{
    const int length = 300;

    for (int i = 0; i <= 90; i++)
    {
        if (i <= 45)
            lineSeries.append(i, length * (1 / ( 1 + std::tan(i / 180.0 * 3.1415))));
        else
            lineSeries.append(i, length * (1 / (1 + std::tan((90 - i) / 180.0 * 3.1415))));
    }

    lineSeries.setName(u8"Зависимость длины ступеньки от угла наклона прямой");

    QFont font = lenAxis.titleFont();
    font.setPointSize(14);

    lenAxis.setTitleText(u8"Длина ступеньки");
    lenAxis.setTitleFont(font);
    angleAxis.setTitleText(u8"Угол наклона отрезка");
    angleAxis.setTitleFont(font);

    chart.setTitle(u8"Анализ ступенчатости");
    chart.setTitleFont(font);
    chart.addSeries(&lineSeries);
    // chart.createDefaultAxes();

    chart.addAxis(&lenAxis, Qt::AlignLeft);
    chart.addAxis(&angleAxis, Qt::AlignBottom);

    lineSeries.attachAxis(&lenAxis);
    lineSeries.attachAxis(&angleAxis);

    chartView.setChart(&chart);
    chartView.setRenderHint(QPainter::Antialiasing);

    ui.horizontalLayout->insertWidget(0, &chartView);
}
