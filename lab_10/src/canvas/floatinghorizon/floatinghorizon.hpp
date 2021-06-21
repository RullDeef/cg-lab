#pragma once

#include <functional>
#include <QPainter>
#include <vector>
#include "Line.hpp"
#include "Math/Matrix.hpp"
#include "viewport.hpp"

using Y_t = std::function<double(double, double)>;


class FloatingHorizon
{
public:
    explicit FloatingHorizon(size_t width);
    ~FloatingHorizon();

    void setWidth(size_t newWidth);
    size_t getWidth() const;

    void setStep(int step);
    int getStep() const;

    void setSurface(Y_t function);
    void setViewport(const Vector& start, const Vector& end, size_t xCount = 20, size_t zCount = 20, size_t nTicks = 1);
    void rotateView(double angleX, double angleY, double angleZ);
    void rotateView(const Matrix& mat);
    void resetRotation();

    void draw(QPainter& painter);

protected:
    void updateLines();
    void normalizeAngles();

    void resetHorizonts();
    void drawLine(QPainter& painter, const Line& line);

    static void findRoot(double& x_root, double& y_root, double x1, double x2, double y11, double y12, double y21, double y22);
    static double interp(double x1, double x2, double y1, double y2, double x);

    bool hasHorizonIntersection(int i);

private:
    size_t width;
    size_t horizonSize;
    int step = 1;

    int* upperHorizon;
    int* lowerHorizon;

    Y_t function = nullptr;
    std::vector<Line> lines;

    Viewport viewport;

    double angleX = 0.0;
    double angleY = 0.0;
    Matrix transformations = Matrix::identity();
};
