#pragma once

#include <QWidget>
#include "ui_circlepage.h"

#include <vector>
#include <memory>
#include <QGraphicsScene>

#include "cirrenitem.hpp"

class CirclePage : public QWidget
{
    Q_OBJECT

public:
    CirclePage(QWidget *parent = Q_NULLPTR);

    void initAlgos(std::vector<std::shared_ptr<CircleRenderer>> algos);

    void mousePressEvent(QMouseEvent* event) override;

public slots:
    inline void setCX(int value) { eXCenter = value; }
    inline void setCY(int value) { eYCenter = value; }
    inline void setCRadius(int value) { eRadius = value; }

    inline void setAlgorithm(int value) { algorithmIndex = value; }

    void selectColor();
    void drawCircle();

    void clearCanvas();

private:
    Ui::CirclePage ui;

    int eXCenter = 0;
    int eYCenter = 0;
    int eRadius = 0;

    QColor color = QColor(100, 255, 100);
    QGraphicsScene colorScene;

    std::vector<std::shared_ptr<CircleRenderer>> circleRenderers;
    int algorithmIndex = 0;

    QGraphicsScene canvasScene;
    CirRenItem* cirRenItem;
};
