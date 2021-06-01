#pragma once

#include "../../core/basecutter.hpp"
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
        void removeSegmentsButtonPressed();
        void cutterSelected(core::BasicRegion& cutter);

    private:
        Ui::cutterTab ui;

        RectCutterCanvas* canvas;
    };
}
