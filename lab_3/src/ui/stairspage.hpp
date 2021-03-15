#pragma once

#include <QWidget>
#include <QtCharts>
#include "ui_stairspage.h"

namespace ui
{
    class StairsPage : public QWidget
    {
        Q_OBJECT

    public:
        StairsPage(QWidget *parent = Q_NULLPTR);
        ~StairsPage();

    private:
        void initChart();

        Ui::StairsPage ui;

        QtCharts::QChartView chartView;
        QtCharts::QChart chart;

        QtCharts::QValueAxis lenAxis;
        QtCharts::QValueAxis angleAxis;

        QtCharts::QLineSeries lineSeries;
    };
}
