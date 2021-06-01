#pragma once

#include <thread>
#include "../../core/regionrenderer.hpp"
#include "base/interactivetabwidget.hpp"
#include "base/basicregionwrapper.hpp"
#include "base/colorpickerwidget.hpp"
#include "base/smartcanvas.hpp"
#include "ui_fillertab.h"


namespace ui
{
    class FillerTab : public InteractiveTabWidget
    {
        Q_OBJECT

    public:
        FillerTab(core::RegionRenderer* renderer);

    public slots:
        void selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

        void specialPointChanged(int x, int y);

        void constraintToggled(bool checked);
        void lineToggled(bool checked);

        void fillButtonPressed();
        void fillStepButtonPressed();
        void clearButtonPressed();
        void removeRegionButtonPressed();
        void clearOverlayButtonPressed();

    protected:
        void showPointEditor();
        void hidePointEditor();

    private:
        Ui::fillerTab ui;

        SmartCanvas* canvas;
        ColorPickerWidget* colorPicker;

        core::RegionRenderer* renderer;
        core::BasicRegion region;
        BasicRegionWrapper regionWrapper;

        PointWrapper selectedPointWrapper;
    };
}
