#include <stdexcept>
#include <cstring>
#include "floatinghorizon.hpp"


FloatingHorizon::FloatingHorizon(size_t width)
    : width(width), horizonSize(width)
{
    if (width == 0)
        throw std::runtime_error("bad width for floating horizon");

    try
    {
        upperHorizon = new int[horizonSize];
        lowerHorizon = new int[horizonSize];
    }
    catch (...)
    {
        throw std::runtime_error("bad alloc in floating horizon");
    }
}

FloatingHorizon::~FloatingHorizon()
{
    delete[] lowerHorizon;
    delete[] upperHorizon;
}

void FloatingHorizon::setWidth(size_t newWidth)
{
    if (width != newWidth)
    {
        if (newWidth == 0)
            throw std::runtime_error("bad width for floating horizon");
        width = newWidth;
        horizonSize = std::max(int(width / step), 1);

        try
        {
            delete[] upperHorizon;
            delete[] lowerHorizon;
            upperHorizon = new int[horizonSize];
            lowerHorizon = new int[horizonSize];
        }
        catch (...)
        {
            throw std::runtime_error("bad alloc in floating horizon");
        }
    }
}

size_t FloatingHorizon::getWidth() const
{
    return width;
}

void FloatingHorizon::setStep(int step)
{
    if (step <= 0)
        throw std::runtime_error("bad step for floating horizon");

    this->step = step;
    horizonSize = std::max(int(width / step), 1);
    try
    {
        delete[] upperHorizon;
        delete[] lowerHorizon;
        upperHorizon = new int[horizonSize];
        lowerHorizon = new int[horizonSize];
    }
    catch (...)
    {
        throw std::runtime_error("bad alloc in floating horizon");
    }
}

int FloatingHorizon::getStep() const
{
    return step;
}

void FloatingHorizon::draw(QPainter& painter, Y_xz_t func, int z_front, int z_back)
{
    resetHorizonts();

    if (z_front < z_back)
        std::swap(z_front, z_back);

    for (int z = z_front; z > z_back; z--)
    {
        int x_prev = 0;
        int y_prev = func(x_prev, z);
        bool y_prev_upper = y_prev < upperHorizon[0];
        bool y_prev_lower = y_prev > lowerHorizon[0];
        bool y_prev_visible = y_prev_upper || y_prev_lower;

        for (int i = 1, x = step; i < horizonSize; i++, x += step)
        {
            int y = func(x, z);
            bool y_upper = y < upperHorizon[i];
            bool y_lower = y > lowerHorizon[i];
            bool y_visible = y_upper || y_lower;

            if ((y_prev_visible && y_visible) || z > z_front - 2)
            {
                //painter.setPen(Qt::black);
                painter.drawLine(x_prev, y_prev, x, y);
            }
            else if ((y_prev_visible ^ y_visible) && step > 2 && !hasHorizonIntersection(i))
            {
                int* horizon = (y_prev_upper || y_upper) ? upperHorizon : lowerHorizon;

                double x_root, y_root;
                findRoot(x_root, y_root, x_prev, x, y_prev, y, horizon[i - 1], horizon[i]);

                if (y_prev_visible)
                    painter.drawLine(x_prev, y_prev, x_root, y_root);
                else
                    painter.drawLine(x_root, y_root, x, y);

                //painter.setPen(y_prev_visible ? Qt::black : Qt::green);
                //painter.drawLine(x_prev, y_prev, x_root, y_root);
                //painter.setPen(y_prev_visible ? Qt::green : Qt::black);
                //painter.drawLine(x_root, y_root, x, y);
            }
            else if (!y_prev_visible && !y_visible)
            {
                //painter.setPen(Qt::green);
                //painter.drawLine(x_prev, y_prev, x, y);
            }

            if (y_prev_upper)
                upperHorizon[i - 1] = y_prev;

            if (y_prev_lower)
                lowerHorizon[i - 1] = y_prev;

            x_prev = x;
            y_prev = y;
            y_prev_upper = y_upper;
            y_prev_lower = y_lower;
            y_prev_visible = y_visible;
        }

        if (y_prev_upper)
            upperHorizon[horizonSize - 1] = y_prev;

        if (y_prev_lower)
            lowerHorizon[horizonSize - 1] = y_prev;
    }
}

void FloatingHorizon::resetHorizonts()
{
    std::memset(upperHorizon, 10000, horizonSize * sizeof(int));
    std::memset(lowerHorizon, 0, horizonSize * sizeof(int));
}

void FloatingHorizon::findRoot(double& x_root, double& y_root, double x1, double x2, double y11, double y12, double y21, double y22)
{
    x_root = x1 + (y11 - y21) * (x2 - x1) / (y22 - y21 - y12 + y11);
    y_root = y11 + (y12 - y11) * (x_root - x1) / (x2 - x1);

    if (x_root < x1 || x2 < x_root)
        __debugbreak();
}

bool FloatingHorizon::hasHorizonIntersection(int i)
{
    int diff1 = upperHorizon[i - 1] - lowerHorizon[i - 1];
    int diff2 = upperHorizon[i] - lowerHorizon[i];

    return diff1 * diff2 < 0;
}
