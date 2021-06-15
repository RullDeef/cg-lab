#pragma once

#include <QWidget>
#include <QImage>
#include "floatinghorizon/floatinghorizon.hpp"


class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas();
    virtual ~Canvas() = default;

    void setSurface(Y_t function);
    void setViewport(double startX, double endX, double startY, double endY, double startZ, double endZ);
    void setCounts(size_t xCount, size_t zCount);
    void rotateView(double angleX, double angleY, double angleZ);

    void drawSurface();
    void clear();

protected slots:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QImage renderTarget;
    
    Vector start;
    Vector end;

    FloatingHorizon floatingHorizon;
};
