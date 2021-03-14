#pragma once

#include <QMainWindow>
#include "ui_appwindow.h"
#include "segmentpage.hpp"
#include "spectrumpage.hpp"
#include "statisticspage.hpp"

namespace ui
{
    class AppWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        AppWindow(QWidget *parent = Q_NULLPTR);
        ~AppWindow();

    public slots:
        void selectSegmentPage();
        void selectSpectrumPage();
        void selectStatisticsPage();

        void updateTiming_BresenhemReal(qreal time);
        void updateTiming_Library(qreal time);

    private:
        Ui::AppWindow ui;

        SegmentPage* segmentPage;
        SpectrumPage* spectrumPage;
        StatisticsPage* statisticsPage;
    };
}
