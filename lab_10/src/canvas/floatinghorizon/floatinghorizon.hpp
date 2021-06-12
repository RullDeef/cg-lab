#pragma once

#include <functional>
#include <QPainter>

using F_t = std::function<double(double, double, double)>;
using Y_t = std::function<double(double, double)>;
using Y_xz_t = std::function<int(int, int)>;


class FloatingHorizon
{
public:
    explicit FloatingHorizon(size_t width);
    ~FloatingHorizon();

    void setWidth(size_t newWidth);
    size_t getWidth() const;

    void setStep(int step);
    int getStep() const;

    void draw(QPainter& painter, Y_xz_t func, int z_front, int z_back);

protected:
    void resetHorizonts();
    void drawLine(QPainter& painter, int x1, int y1, int x2, int y2);

    static void findRoot(double& x_root, double& y_root, double x1, double x2, double y11, double y12, double y21, double y22);

    bool hasHorizonIntersection(int i);

private:
    size_t width;
    size_t horizonSize;
    int step = 1;
    int* upperHorizon;
    int* lowerHorizon;
};
