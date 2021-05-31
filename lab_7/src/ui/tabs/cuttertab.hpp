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

    public slots:
        void constraintToggled(bool checked);
        void cutterSelected(core::Rect& rect);

        void clearButtonPressed();
        void removeSegmentsButtonPressed();

    protected slots:

    private:
        Ui::cutterTab ui;

        RectCutterCanvas* canvas;
    };
}
