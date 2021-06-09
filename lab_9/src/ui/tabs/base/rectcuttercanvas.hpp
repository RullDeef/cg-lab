#pragma once

#include "core/basecutter.hpp"
#include "core/regionrenderer.hpp"
#include "core/builder/BasicRegionBuilder.hpp"
#include "canvas.hpp"


namespace ui
{
    class RectCutterCanvas : public Canvas
    {
        Q_OBJECT

    public:
        RectCutterCanvas();
        virtual ~RectCutterCanvas() = default;

        inline core::BasicRegion& getCutter() { return cutter; }
        bool cutterIsValid() const;

        void removeRegion();

    signals:
        void cutterSelected(core::BasicRegion& cutter);
        void contourAdded(core::BasicRegion& region);

    public slots:
        void applyCut();
        void clearCut();
        void selectCutter();
        void addRegion();

    protected slots:
        virtual void paintEvent(QPaintEvent* event) override;

        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;

        virtual void enterEvent(QEnterEvent* event) override;
        virtual void leaveEvent(QEvent* event) override;

    protected:
        void normalizeCutter();

        void repaintCanvas();
        void drawCutter(QPainter& painter);
        void drawRegion(QPainter& painter);
        void drawCursor(QPainter& painter);
        void drawConstraints(QPainter& painter);

    private:
        QPoint mousePos;
        bool mouseEntered = false;

        QImage overlay;
        bool selectingCutter = false;
        bool selectingContour = false;

        core::BasicRegionBuilder cutterBuilder;
        core::BasicRegionBuilder regionBuilder;

        core::BasicRegion cutter;
        core::BasicRegion region;
        core::BasicRegion regionCutted;
    };
}
