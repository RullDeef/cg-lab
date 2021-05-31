#pragma once

#include <list>
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
        SmartCanvas();
        virtual ~SmartCanvas();

        void setColor(QColor newColor) { color = newColor; }
        void fillRegion(core::BucFillRenderer& renderer, const QColor& color);
        void fillRegionWithStep(core::AsyncBucFillRenderer* renderer, const QColor& color);

        void clearCanvas();
        void clearRegion();
        void clearOverlay();

        void addContourPoint(int x, int y);
        void changeSpecialPointPos(int x, int y);

    signals:
        void specialPointPosChanged(int x, int y);

    public slots:
        void specialPointSelectAction();
        void closeContour();

    protected slots:
        virtual void keyPressEvent(QKeyEvent* event) override;

        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;

        virtual void enterEvent(QEvent* event) override { mouseEntered = true; update(); }
        virtual void leaveEvent(QEvent* event) override { mouseEntered = false; update(); }

        virtual void resizeEvent(QResizeEvent* event) override;
        virtual void paintEvent(QPaintEvent* event) override;

        void rendererLoop();

    protected:
        void repaintCanvas();
        void drawRegion(QPainter& painter);
        void drawCursor(QPainter& painter);
        void drawAxes(QPainter& painter);
        void drawSpecialPoint(QPainter& painter);

    private:
        bool mouseEntered = false;
        bool mousePressed = false;
        bool specialPointSelectingState = false;

        QPoint mousePos;
        QPoint specialPointPos = QPoint(0, 0);

        std::list<std::list<QPoint>> region;
        QImage fillOverlay;

        QTimer* timer;
        QColor color;
        core::AsyncBucFillRenderer* asyncRenderer = nullptr;
    };
}
