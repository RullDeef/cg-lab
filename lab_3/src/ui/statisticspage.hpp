#pragma once

#include <QWidget>
#include <QtCharts>
#include "ui_statisticspage.h"

#include <list>
#include "../core/segmentrenderer.hpp"

namespace ui
{
    class StatisticsPage : public QWidget
    {
        Q_OBJECT

    public:
        StatisticsPage(QWidget *parent = Q_NULLPTR);

        void initAlgos(const std::list<core::SegmentRenderer*>& algos);

        void useAlgosOnce();

    public slots:
        void updateBarValues();

    private:
        void initChart();
        void setBarValue(int index, qreal value);
        void updateRange();

        Ui::StatisticsPage ui;

        std::list<core::SegmentRenderer*> segmentRenderers;

        QtCharts::QChartView chartView;
        QtCharts::QChart chart;

        QtCharts::QBarCategoryAxis catAxis;
        QtCharts::QValueAxis valAxis;

        QtCharts::QBarSeries series;
        QtCharts::QBarSet barSet;
    };
}
