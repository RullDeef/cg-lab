#pragma once

#include <QWidget>
#include "ui_ellipsepage.h"

#include <vector>
#include <memory>
#include <QGraphicsScene>

#include "ellrenitem.hpp"

class EllipsePage : public QWidget
{
    Q_OBJECT

public:
    EllipsePage(QWidget *parent = Q_NULLPTR);

    void initAlgos(std::vector<std::shared_ptr<EllipseRenderer>> algos);

    void mousePressEvent(QMouseEvent* event) override;

public slots:
    inline void setEX(int value) { eXCenter = value; }
    inline void setEY(int value) { eYCenter = value; }
    inline void setEWidth(int value) { eWidth = value; }
    inline void setEHeight(int value) { eHeight = value; }

    inline void setAlgorithm(int value) { algorithmIndex = value; }

    void selectColor();
    void drawEllipse();

    void clearCanvas();

private:
    Ui::EllipsePage ui;

    int eXCenter = 0;
    int eYCenter = 0;
    int eWidth = 0;
    int eHeight = 0;

    QColor color = QColor(100, 255, 100);
    QGraphicsScene colorScene;

    std::vector<std::shared_ptr<EllipseRenderer>> ellipseRenderers;
    int algorithmIndex = 0;

    QGraphicsScene canvasScene;
    EllRenItem* ellRenItem;
};
