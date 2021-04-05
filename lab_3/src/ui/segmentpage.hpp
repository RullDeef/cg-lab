#pragma once

#include <QWidget>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <vector>
#include "ui_segmentpage.h"
#include "segrenitem.hpp"
#include "axisrenitem.hpp"
#include "../core/segmentrenderer.hpp"

namespace ui
{
    class SegmentPage : public QWidget
    {
        Q_OBJECT

    public:
        SegmentPage(QWidget *parent = Q_NULLPTR);
        ~SegmentPage();

        void initAlogs(const std::list<core::SegmentRenderer*>& algos);

        void mousePressEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent *event) override;

    public slots:
        inline void setX1(int value) { x1 = value; }
        inline void setY1(int value) { y1 = value; }
        inline void setX2(int value) { x2 = value; }
        inline void setY2(int value) { y2 = value; }

        inline void setAlgorithm(int value) { algorithmIndex = value; }

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

        std::vector<core::SegmentRenderer*> segmentRenderers;
        int algorithmIndex = 0;

        QGraphicsScene canvasScene;
        SegRenItem* segRenItem;
        AxisRenItem* axisRenItem;
    };
}
