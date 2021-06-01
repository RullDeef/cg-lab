#pragma once

#include <QWidget>
#include <QImage>
#include <functional>

using F_t = std::function<double(double,double,double)>;

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas();
    virtual ~Canvas() = default;

    virtual void drawSurface(F_t surface, double startX, double endX, double startY, double endY, double startZ, double endZ);
    virtual void clear();

protected slots:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QImage renderTarget;
};
