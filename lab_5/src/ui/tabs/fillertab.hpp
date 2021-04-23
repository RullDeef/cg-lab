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
        FillerTab(core::RegionRenderer* renderer);

    public slots:
        void selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

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
