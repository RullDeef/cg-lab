#pragma once

#include "../../../core/basecutter.hpp"
#include "canvas.hpp"


namespace ui
{
    class RectCutterCanvas : public Canvas
    {
        Q_OBJECT

    public:
        RectCutterCanvas();
        virtual ~RectCutterCanvas() = default;

        core::Rect& getCutterRect() { return rect; }

    signals:
        void cutterSelected(core::Rect& rect);

    public slots:
        void applyCut();
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

    private:
        QPoint mousePos;
        bool mouseEntered = false;

        QImage overlay;
        bool selectingRect = false;
        bool drawingLine = false;

        core::Rect rect;

        core::Segment tempLine;
        std::list<core::Segment> lines;
        std::list<core::Segment> linesCutted;
    };
}
