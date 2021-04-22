#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidget>
#include "base/interactivetabwidget.hpp"
#include "base/canvastabwidget.hpp"
#include "../../core/regionrenderer.hpp"
#include "ui_fillertab.h"

namespace ui
{
    class PointWrapper : public QObject
    {
        Q_OBJECT

    public:
        PointWrapper() = default;
        PointWrapper(core::BasicRegion::Point& point) : point(&point) {}
        virtual ~PointWrapper();

        void wrap(core::BasicRegion::Point& point) { this->point = &point; }

        void setX(int x) { point->x = x; emit modified(); }
        void setY(int y) { point->y = y; emit modified(); }
        void setXY(int x, int y) { point->x = x; point->y = y; emit modified(); }

        core::BasicRegion::Point& operator*() { return *point; }

    signals:
        void modified();

    private:
        core::BasicRegion::Point* point = nullptr;
    };

    class BasicRegionWrapper : public QObject
    {
        Q_OBJECT

    public:
        BasicRegionWrapper(core::BasicRegion& region) : region(region) {}

        core::BasicRegion& operator*() { return region; }

    signals:
        void pointAdded(core::BasicRegion::Point& point);

    public slots:
        core::BasicRegion::Point& appendPoint(int x, int y);

    private:
        core::BasicRegion& region;
    };

    class CanvasRendererItem : public QGraphicsItem
    {
    public:
        CanvasRendererItem(QSize size = QSize(1, 1));

        void resizeImage(QSize size);

        QRectF boundingRect() const override { return QRectF(QPointF(0, 0), image.size()); }
        QImage& getImage() { return image; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR) override;

    private:
        QImage image;
    };

    class Canvas : public QGraphicsView
    {
        Q_OBJECT

    public:
        Canvas();
        virtual ~Canvas() { delete canvasRenderer; }

    protected slots:
        virtual void resizeEvent(QResizeEvent* event) override;
        virtual void paintEvent(QPaintEvent* event) override;

    protected:
        QImage& getImage() { return canvasRenderer->getImage(); }

    private:
        QGraphicsScene scene;
        CanvasRendererItem* canvasRenderer;
    };

    class PointSelector : public QObject
    {
        Q_OBJECT

    public:
        PointSelector(BasicRegionWrapper& region) : region(region) {}

        void selectPoint(int x, int y);
        void deselectPoint() { emit selectionChanged(selectedPoint, nullptr); selectedPoint = nullptr; }
        bool selected() const { return selectedPoint; }

        core::BasicRegion::Point* getSelected() { return selectedPoint; }

        bool removePointAt(int x, int y);

        void drawSelection(QPainter& painter) const;

    signals:
        void selectionChanged(core::BasicRegion::Point* oldSelectedPoint, core::BasicRegion::Point* newSelectedPoint);

    protected:
        core::BasicRegion::Point* pointAt(int x, int y);

    private:
        BasicRegionWrapper& region;
        core::BasicRegion::Point* selectedPoint = nullptr;
    };

    class LineConnector : public QObject
    {
        Q_OBJECT

    public:
        LineConnector(BasicRegionWrapper& region) : region(region) {}

        void enable() { enabled = true; }
        void disable() { enabled = false; }
        void toggle() { enabled = !enabled; }

        bool removeLineAt(int x, int y);

        void drawHelpers(QPainter& painter, const QPoint& mousePos) const {}

    public slots:
        void selectionChanged(core::BasicRegion::Point* oldSelectedPoint, core::BasicRegion::Point* newSelectedPoint);

    private:
        bool enabled = true;
        BasicRegionWrapper& region;
    };

    class PointConstrainter
    {
    public:
        PointConstrainter(BasicRegionWrapper& region) : region(region) {}

        void enable() { enabled = true; }
        void disable() { enabled = false; }
        void toggle() { enabled = !enabled; }
        QPoint constraint(const QPoint& point);

        void drawConstraints(QPainter& painter, const QPoint& point);

    private:
        BasicRegionWrapper& region;

        int stickWidth = 12;
        bool enabled = true;
    };

    class SmartCanvas : public Canvas
    {
        Q_OBJECT

    public:
        SmartCanvas(BasicRegionWrapper& region);
        virtual ~SmartCanvas();

        void clearCanvas();

    signals:
        void selectionChanged(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

    public slots:
        void regionModified();

    protected slots:
        virtual void keyPressEvent(QKeyEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void enterEvent(QEvent* event) override { mouseEntered = true; update(); }
        virtual void leaveEvent(QEvent* event) override { mouseEntered = false; update(); }

        virtual void paintEvent(QPaintEvent* event) override;

        void selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

    protected:
        void repaintCanvas();
        void drawRegion(QPainter& painter);
        void drawCursor(QPainter& painter);
        void drawAxes(QPainter& painter);

    private:
        BasicRegionWrapper& region;
        QPoint mousePos;
        bool mouseEntered = false;

        PointConstrainter pointConstrainter;
        PointSelector pointSelector;
        LineConnector lineConnector;
    };

    class FillerTab : public InteractiveTabWidget
    {
        Q_OBJECT

    public:
        FillerTab(core::RegionRenderer* renderer);

    public slots:
        void selectionChangedSlot(core::BasicRegion::Point* oldPoint, core::BasicRegion::Point* newPoint);

        void clearButtonPressed();

    protected:
        void showPointEditor();
        void hidePointEditor();

    private:
        Ui::fillerTab ui;

        SmartCanvas* canvas;

        core::RegionRenderer* renderer;
        core::BasicRegion region;
        BasicRegionWrapper regionWrapper;

        PointWrapper selectedPointWrapper;
    };
}
