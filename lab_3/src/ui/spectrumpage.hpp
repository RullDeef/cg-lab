#pragma once

#include <QWidget>
#include <vector>
#include "ui_spectrumpage.h"
#include "segrenitem.hpp"
#include "axisrenitem.hpp"
#include "../core/segmentrenderer.hpp"

namespace ui
{
    class SpectrumPage : public QWidget
    {
        Q_OBJECT

    public:
        SpectrumPage(QWidget *parent = Q_NULLPTR);
        ~SpectrumPage();

        void initAlgos(const std::list<core::SegmentRenderer*>& algos);

        void mousePressEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent* event) override;

    public slots:
        inline void setLength(int value) { segmentsLength = value; }
        inline void setAmount(int value) { segmentsAmount = value; }

        inline void setAlgorithm1(int value) { algorithm1Index = value; }
        inline void setAlgorithm2(int value) { algorithm2Index = value; }

        void selectColor();
        void clearCanvas();

        inline void drawSpectrum1() { drawSpectrum(segmentRenderers[algorithm1Index], colorFg); }
        inline void drawSpectrum2() { drawSpectrum(segmentRenderers[algorithm2Index], colorBg); }

    protected:
        void drawSpectrum(core::SegmentRenderer* renderer, QColor color);

    private:
        Ui::SpectrumPage ui;

        int segmentsLength = 1;
        int segmentsAmount = 1;

        QColor colorFg = QColor(100, 255, 100);
        QColor colorBg = Qt::white;
        QGraphicsScene colorScene;
        
        std::vector<core::SegmentRenderer*> segmentRenderers;
        int algorithm1Index = 0;
        int algorithm2Index = 0;

        QGraphicsScene canvasScene;
        SegRenItem* segRenItem;
        AxisRenItem* axisRenItem;
    };
}
