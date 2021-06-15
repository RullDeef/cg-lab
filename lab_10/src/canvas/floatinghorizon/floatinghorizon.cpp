#include <stdexcept>
#include <cstring>
#include "floatinghorizon.hpp"
#include "Math/Matrix.hpp"

constexpr auto SQRT1_2 = 0.70710678;


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

void FloatingHorizon::setSurface(Y_t function)
{
    this->function = function;
    lines.clear();
}

void FloatingHorizon::setViewport(const Vector& start, const Vector& end, size_t xCount, size_t zCount)
{
    if (!function)
        throw std::runtime_error("function must be set");

    lines.clear();
    lines.reserve(2 * xCount * zCount - xCount - zCount);

    auto dir = end - start;
    auto delta = end - start;
    auto center = dir / 2;
    delta.setX(delta.getX() / (xCount - 1));
    delta.setZ(delta.getZ() / (zCount - 1));

    auto projectorX = [=](double x) {
        auto f = (x - start.getX()) / dir.getX();
        return width * (f * SQRT1_2 + 0.5 * (1 - SQRT1_2));
    };
    auto projectorZ = [=](double z) {
        auto f = (z - start.getZ()) / dir.getZ();
        return width * (f * SQRT1_2 + 0.5 * (1 - SQRT1_2));
    };

    for (size_t i = 0; i < xCount; i++)
    {
        double x = start.getX() + delta.getX() * i;
        double xp = projectorX(x);
        for (size_t j = 0; j < zCount; j++)
        {
            double z = start.getZ() + delta.getZ() * j;
            double zp = projectorZ(z);
            double y = function(x, z);

            if (i < xCount - 1)
            {
                double x2 = x + delta.getX();
                double xp2 = projectorX(x2);
                double y2 = function(x2, z);

                lines.push_back(Line(Vector(xp, y, zp), Vector(xp2, y2, zp)));
            }
            if (j < zCount - 1)
            {
                double z2 = z + delta.getZ();
                double zp2 = projectorZ(z2);
                double y2 = function(x, z2);

                lines.push_back(Line(Vector(xp, y, zp), Vector(xp, y2, zp2)));
            }
        }
    }
}

void FloatingHorizon::rotateView(double angleX, double angleY, double angleZ)
{
    auto mat = Matrix::rotation(Vector(1.0, 0.0, 0.0), angleX) *
        Matrix::rotation(Vector(0.0, 1.0, 0.0), angleY) *
        Matrix::rotation(Vector(0.0, 0.0, 1.0), angleZ);

    double centerY = 0.0;
    for (const auto& line : lines)
        centerY += line.center().getY();
    centerY /= lines.size();

    auto delta = Vector(width / 2, centerY, width / 2);
    for (auto& line : lines)
    {
        line.p1 = (line.p1 - delta) * mat + delta;
        line.p2 = (line.p2 - delta) * mat + delta;

        if (line.p2.getX() < line.p1.getX())
            std::swap(line.p1, line.p2);

        //double m = (line.p2 - line.p1).getY() / (line.p2 - line.p1).getX();
        //if (line.p1.getX() < 0)
        //{
        //    line.p1.setY(line.p1.getX() * (1 - m));
        //    line.p1.setX(0);
        //}
        //if (line.p2.getX() >= width)
        //{
        //    line.p2.setY(line.p1.getX() * (1 + m) + m * (width - 1));
        //    line.p2.setX(width - 1);
        //}
    }

    std::sort(lines.begin(), lines.end(),
        [](const Line& l1, const Line& l2) { return l1.z() < l2.z(); });
}

void FloatingHorizon::draw(QPainter& painter)
{
    resetHorizonts();
    for (const auto& line : lines)
        drawLine(painter, line);
}

/*
void draw(QPainter& painter, Y_xz_t func, int z_front, int z_back)
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
*/

void FloatingHorizon::resetHorizonts()
{
    std::memset(upperHorizon, 10000, horizonSize * sizeof(int));
    std::memset(lowerHorizon, 0, horizonSize * sizeof(int));
}

#define TEST_DBG { if (i1 >= width || i2 >= width) __debugbreak(); }

void FloatingHorizon::drawLine(QPainter& painter, const Line& line)
{
    if ((line.p2 - line.p1).getX() < step)
    {
        painter.setPen(Qt::red);
        painter.drawLine(line.p1.getX(), line.p1.getY(), line.p2.getX(), line.p2.getY());
        return;
    }

    auto delta = (line.p2 - line.p1) / (line.p2 - line.p1).getX() * step;

    for (Vector p = line.p1; p.getX() < line.p2.getX(); p = p + delta)
    {
        long i = p.getX() / step;

        if (i >= 0 && i < width)
        {
            bool visible = false;
            if (p.getY() < upperHorizon[i])
            {
                upperHorizon[i] = p.getY();
                visible = true;
            }
            if (p.getY() > lowerHorizon[i])
            {
                lowerHorizon[i] = p.getY();
                visible = true;
            }

            if (visible)
            {
                painter.setPen(Qt::black);
                painter.drawLine(p.getX(), p.getY(), (p + delta).getX(), (p + delta).getY());
            }
        }
    }

    // painter.drawLine(line.p1.getX(), line.p1.getY(), line.p2.getX(), line.p2.getY());
}

/*
void FloatingHorizon::drawLine(QPainter& painter, const Line& line)
{
    size_t i1 = line.p1.getX() / step;
    size_t i2 = i1 + 1;

    Vector delta = line.p2 - line.p1;
    double dy = delta.getY() / delta.getX() * step;

    double x1 = line.p1.getX();
    double y1 = line.p1.getY();
    double x2 = x1 + step;
    double y2 = y1 + dy;

    TEST_DBG;
    bool prev_upper = y1 < upperHorizon[i1];
    bool prev_lower = y1 > lowerHorizon[i1];

    for (; x2 < line.p2.getX(); i1++, i2++)
    {
        TEST_DBG;
        bool upper = y2 < upperHorizon[i2];
        bool lower = y2 > lowerHorizon[i2];

        if ((prev_upper && prev_lower) && (upper && lower))
        {
            painter.setPen(Qt::black);
            painter.drawLine(x1, y1, x2, y2);
        }
        else
        {
            painter.setPen(Qt::green);
            painter.drawLine(x1, y1, x2, y2);
        }

        if (prev_upper)
            upperHorizon[i1] = y1;

        if (prev_lower)
            lowerHorizon[i1] = y1;

        prev_lower = lower;
        prev_upper = upper;

        x1 = x2;
        y1 = y2;
        x2 += step;
        y2 += dy;
    }

    TEST_DBG;
    if (prev_upper)
        upperHorizon[i1] = y1;

    if (prev_lower)
        lowerHorizon[i1] = y1;

    //painter.drawLine(line.p1.getX(), line.p1.getY(), line.p2.getX(), line.p2.getY());
}
*/

void FloatingHorizon::findRoot(double& x_root, double& y_root, double x1, double x2, double y11, double y12, double y21, double y22)
{
    x_root = x1 + (y11 - y21) * (x2 - x1) / (y22 - y21 - y12 + y11);
    y_root = y11 + (y12 - y11) * (x_root - x1) / (x2 - x1);

    if (x_root < x1 || x2 < x_root)
        __debugbreak();
}

double FloatingHorizon::interp(double x1, double x2, double y1, double y2, double x)
{
    return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
}

bool FloatingHorizon::hasHorizonIntersection(int i)
{
    int diff1 = upperHorizon[i - 1] - lowerHorizon[i - 1];
    int diff2 = upperHorizon[i] - lowerHorizon[i];

    return diff1 * diff2 < 0;
}
