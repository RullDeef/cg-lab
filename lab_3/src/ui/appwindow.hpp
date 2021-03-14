#pragma once

#include <QMainWindow>
#include <list>
#include "ui_appwindow.h"
#include "segmentpage.hpp"
#include "spectrumpage.hpp"
#include "statisticspage.hpp"

#include "../core/segmentrenderer.hpp"

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

    private:
        Ui::AppWindow ui;

        std::list<core::SegmentRenderer*> segmentRenderers;

        SegmentPage* segmentPage;
        SpectrumPage* spectrumPage;
        StatisticsPage* statisticsPage;
    };
}
