#pragma once

#include <QWidget>
#include "segmentrenderer.hpp"
#include "ui_spectrumpage.h"

namespace ui
{
    class SpectrumPage : public QWidget
    {
        Q_OBJECT

    public:
        SpectrumPage(QWidget *parent = Q_NULLPTR);
        ~SpectrumPage();

        void mousePressEvent(QMouseEvent* event) override;

    public slots:
        inline void setLength(int value) { segmentsLength = value; }
        inline void setAngleStep(int value)
        {
            angleStep = value;
            ui.amountInput->blockSignals(true);
            ui.amountInput->setValue(360.0 / value);
            ui.amountInput->blockSignals(false);
        }
        
        inline void setAmount(int value)
        {
            angleStep = 360.0 / value;
            ui.stepInput->blockSignals(true);
            ui.stepInput->setValue(angleStep);
            ui.stepInput->blockSignals(false);
        }

        inline void setAlgorithm1(int value) { algorithm1 = (SegmentRenderer::Algorithm)value; }
        inline void setAlgorithm2(int value) { algorithm2 = (SegmentRenderer::Algorithm)value; }

        void selectColor();
        void clearCanvas();

        inline void drawSpectrum1() { drawSpectrum(algorithm1, colorFg); }
        inline void drawSpectrum2() { drawSpectrum(algorithm2, colorBg); }

    protected:
        void drawSpectrum(SegmentRenderer::Algorithm algorithm, QColor color);

    private:
        Ui::SpectrumPage ui;

        int segmentsLength = 1;
        int angleStep = 1;

        QColor colorFg = QColor(100, 255, 100);
        QColor colorBg = Qt::white;
        QGraphicsScene colorScene;

        SegmentRenderer::Algorithm algorithm1 = SegmentRenderer::Algorithm::DDA;
        SegmentRenderer::Algorithm algorithm2 = SegmentRenderer::Algorithm::DDA;

        QGraphicsScene canvasScene;
        SegmentRenderer* segmentRenderer;
    };
}
