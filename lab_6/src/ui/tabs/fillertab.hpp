#pragma once

#include <thread>
#include "base/interactivetabwidget.hpp"
#include "../../core/regionrenderer.hpp"
#include "fillertab/basicregionwrapper.hpp"
#include "fillertab/colorpickerwidget.hpp"
#include "fillertab/smartcanvas.hpp"
#include "ui_fillertab.h"


namespace ui
{
    class FillerTab : public InteractiveTabWidget
    {
        Q_OBJECT

    public:
        FillerTab(core::AsyncBucFillRenderer* renderer);

    public slots:
        void specialPointChanged(int x, int y);
        void addContourPoint();

        void fillButtonPressed();
        void fillStepButtonPressed();
        void clearButtonPressed();
        void removeRegionButtonPressed();
        void clearOverlayButtonPressed();
        void colorPicked(QColor color);

    private:
        Ui::fillerTab ui;

        SmartCanvas* canvas;
        ColorPickerWidget* colorPicker;

        core::AsyncBucFillRenderer* renderer;
    };
}
