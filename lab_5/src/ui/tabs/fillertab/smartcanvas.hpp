#pragma once

#include "canvas.hpp"
#include "basicregionwrapper.hpp"
#include "pointselector.hpp"
#include "lineconnector.hpp"
#include "pointconstrainter.hpp"

namespace ui
{
    class SmartCanvas : public Canvas
    {
        Q_OBJECT

    public:
        SmartCanvas(BasicRegionWrapper& region);
        virtual ~SmartCanvas();

        long long fillRegion(core::RegionRenderer& renderer, const QColor& color);
        void fillRegionWithStep(core::AsyncRegionRenderer* renderer, const QColor& color);

        void clearCanvas();
        void clearOverlay();

        void deselect() { pointSelector.deselectPoint(); }

        void setConstraintEnabled(bool enabled);
        void setLineEnabled(bool enabled);

    signals:
        void selectionChanged(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

        void constraintToggled(bool checked);
        void lineToggled(bool checked);

    public slots:
        void regionModified();

    protected slots:
        virtual void keyPressEvent(QKeyEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void enterEvent(QEvent* event) override { mouseEntered = true; update(); }
        virtual void leaveEvent(QEvent* event) override { mouseEntered = false; update(); }

        virtual void resizeEvent(QResizeEvent* event) override;
        virtual void paintEvent(QPaintEvent* event) override;

        void rendererLoop();

        void selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

    protected:
        void repaintCanvas();
        void drawRegion(QPainter& painter);
        void drawCursor(QPainter& painter);
        void drawAxes(QPainter& painter);
        void drawBound(QPainter& painter);

    private:
        bool mouseEntered = false;
        QPoint mousePos;
        BasicRegionWrapper& region;
        QImage fillOverlay;

        QTimer* timer;
        QColor color;
        core::AsyncRegionRenderer* asyncRenderer = nullptr;

        PointConstrainter pointConstrainter;
        PointSelector pointSelector;
        LineConnector lineConnector;
    };
}
