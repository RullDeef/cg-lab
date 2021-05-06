#pragma once

#include <list>
#include <QtCharts>
#include "widgetoptions.hpp"
#include "interactivetabwidget.hpp"
#include "ui_charttabwidget.h"

namespace ui
{
    class ChartTabWidget : public InteractiveTabWidget
    {
        Q_OBJECT
            
    public:
        ChartTabWidget(const char* name, const char* chartTitle, QWidget* parent = Q_NULLPTR);
        virtual ~ChartTabWidget() {}

    protected:
        QtCharts::QChart& getChart() { return chart; }
        QWidget* getWidget() { return (QWidget*)&ui; }
        QHBoxLayout* getLayout() { return ui.horizontalLayout; }

    private:
        void initChart(const char* title);

        Ui::ChartTabWidget ui;

        QtCharts::QChartView chartView;
        QtCharts::QChart chart;
    };

    class LineChartTabWidget : public ChartTabWidget
    {
    public:
        LineChartTabWidget(const char* name, const char* chartTitle, QWidget* parent = Q_NULLPTR);
        virtual ~LineChartTabWidget();

    protected:
        void addPoint(const QString& seriesName, qreal x, qreal y);
        void refreshSeries();

        void clearSeries();

    private:
        void initChart();

        QtCharts::QValueAxis xAxis;
        QtCharts::QValueAxis yAxis;

        std::map<QString, QtCharts::QLineSeries*> series;
    };
}
