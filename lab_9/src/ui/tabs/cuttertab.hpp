#pragma once

#include "core/basecutter.hpp"
#include "base/basicregionwrapper.hpp"
#include "base/interactivetabwidget.hpp"
#include "base/rectcuttercanvas.hpp"
#include "base/basicregionwrapper.hpp"
#include "ui_cuttertab.h"
#include <list>


namespace ui
{
    class CutterTab : public InteractiveTabWidget
    {
        Q_OBJECT

    public:
        CutterTab();

    protected slots:
        void removeRegionButtonPressed();
        void cutterSelected(core::BasicRegion& cutter);
        void contourAdded(core::BasicRegion& region);

    private:
        Ui::cutterTab ui;

        RectCutterCanvas* canvas;
    };
}
