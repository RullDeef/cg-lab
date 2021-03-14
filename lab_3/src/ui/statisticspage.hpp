#pragma once

#include <QWidget>
#include <QtCharts>
#include "ui_statisticspage.h"

namespace ui
{
    class StatisticsPage : public QWidget
    {
        Q_OBJECT

    public:
        StatisticsPage(QWidget *parent = Q_NULLPTR);

        void initBars(const QStringList& strings);
        void setBarValue(int index, qreal value);

    private:
        void initChart();

        Ui::StatisticsPage ui;

        QtCharts::QChartView chartView;
        QtCharts::QChart chart;

        QtCharts::QBarCategoryAxis catAxis;
        QtCharts::QValueAxis valAxis;

        QtCharts::QBarSeries series;
        QtCharts::QBarSet barSet;
    };
}
