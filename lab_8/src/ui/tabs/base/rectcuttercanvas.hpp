#pragma once

#include "core/basecutter.hpp"
#include "core/regionrenderer.hpp"
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

        void removeSegments();

    signals:
        void cutterSelected(core::BasicRegion& cutter);

    public slots:
        void applyCut();
        void clearCut();
        void selectCutter();

    protected slots:
        virtual void paintEvent(QPaintEvent* event) override;

        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;

        virtual void enterEvent(QEvent* event) override;
        virtual void leaveEvent(QEvent* event) override;

    protected:
        void beginLine(int x, int y);
        void continueLine(int x, int y);
        void endLine();

        void normalizeCutter();

        void repaintCanvas();
        void drawCutter(QPainter& painter);
        void drawLines(QPainter& painter);
        void drawCursor(QPainter& painter);
        void drawConstraints(QPainter& painter);

    private:
        QPoint mousePos;
        bool mouseEntered = false;

        QImage overlay;
        bool selectingCutter = false;
        bool drawingLine = false;

        core::BasicRegion cutter;
        core::Point* lastPoint = nullptr;

        core::Point startPoint;
        core::Segment tempLine;
        std::list<core::Segment> lines;
        std::list<core::Segment> linesCutted;
    };
}
