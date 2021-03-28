#pragma once

#include <QWidget>
#include <QtCharts>
#include <vector>
#include <list>
#include "../core/segmentrenderer.hpp"
#include "ui_stairspage.h"

namespace ui
{
    class StairsPage : public QWidget
    {
        Q_OBJECT

    public:
        StairsPage(QWidget *parent = Q_NULLPTR);
        ~StairsPage();

        void InitAlgos(const std::list<core::SegmentRenderer*>& algos);

    private:
        int calcStairsCount(double segLength, int angleDeg);
        void initChart();

        Ui::StairsPage ui;

        QtCharts::QChartView chartView;
        QtCharts::QChart chart;

        QtCharts::QValueAxis lenAxis;
        QtCharts::QValueAxis angleAxis;

        std::vector<QtCharts::QLineSeries*> lineSeries;
        std::vector<core::SegmentRenderer*> segmentRenderers;
    };
}
