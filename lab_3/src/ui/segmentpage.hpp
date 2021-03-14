#pragma once

#include <QWidget>
#include <QImage>
#include <QGraphicsPixmapItem>
#include "segmentrenderer.hpp"
#include "ui_segmentpage.h"

namespace ui
{
    class SegmentPage : public QWidget
    {
        Q_OBJECT

    public:
        SegmentPage(QWidget *parent = Q_NULLPTR);
        ~SegmentPage();

        void mousePressEvent(QMouseEvent* event) override;

    public slots:
        inline void setX1(int value) { x1 = value; }
        inline void setY1(int value) { y1 = value; }
        inline void setX2(int value) { x2 = value; }
        inline void setY2(int value) { y2 = value; }

        inline void setAlgorithm(int value) { algorithm = (SegmentRenderer::Algorithm)value; }

        void selectColor();
        void drawSegment();

        void clearCanvas();

    private:
        Ui::SegmentPage ui;

        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        QColor color = QColor(100, 255, 100);
        QGraphicsScene colorScene;

        SegmentRenderer::Algorithm algorithm = SegmentRenderer::Algorithm::DDA;

        QGraphicsScene canvasScene;
        SegmentRenderer* segmentRenderer;
    };
}
