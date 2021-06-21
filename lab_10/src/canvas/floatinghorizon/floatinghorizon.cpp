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

    rotateView(transformations);
}

void FloatingHorizon::setViewport(const Vector& start, const Vector& end, size_t xCount, size_t zCount, size_t nTicks)
{
    viewport.xStart = start.getX();
    viewport.yStart = start.getY();
    viewport.zStart = start.getZ();

    viewport.xEnd = end.getX();
    viewport.yEnd = end.getY();
    viewport.zEnd = end.getZ();

    viewport.xCount = xCount;
    viewport.zCount = zCount;
    viewport.nTicks = nTicks;

    if (function)
        rotateView(transformations);
}

void FloatingHorizon::rotateView(double angleX, double angleY, double angleZ)
{
    this->angleX += angleX;
    this->angleY += angleY;

    normalizeAngles();

    transformations =
        Matrix::rotation(Vector(0.0, 1.0, 0.0), this->angleY) *
        Matrix::rotation(Vector(1.0, 0.0, 0.0), this->angleX);

    rotateView(transformations);
}

void FloatingHorizon::rotateView(const Matrix& mat)
{
    updateLines();

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
    }

    if (0 <= angleY && angleY < qDegreesToRadians(45))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_j == l2.index_j) return l1.index_i > l2.index_i;
            return l1.index_j < l2.index_j;
        });
    }
    else if (qDegreesToRadians(45) <= angleY && angleY < qDegreesToRadians(90))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_i == l2.index_i) return l1.index_j > l2.index_j;
            return l1.index_i > l2.index_i;
        });
    }
    else if (qDegreesToRadians(90) <= angleY && angleY < qDegreesToRadians(135))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_i == l2.index_i) return l1.index_j < l2.index_j;
            return l1.index_i > l2.index_i;
        });
    }
    else if (qDegreesToRadians(135) <= angleY && angleY < qDegreesToRadians(180))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_j == l2.index_j) return l1.index_i < l2.index_i;
            return l1.index_j > l2.index_j;
        });
    }
    else if (qDegreesToRadians(180) <= angleY && angleY < qDegreesToRadians(225))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_j == l2.index_j) return l1.index_i > l2.index_i;
            return l1.index_j > l2.index_j;
        });
    }
    else if (qDegreesToRadians(225) <= angleY && angleY < qDegreesToRadians(270))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_j == l2.index_j) return l1.index_i < l2.index_i;
            return l1.index_j > l2.index_j;
        });
    }
    else if (qDegreesToRadians(270) <= angleY && angleY < qDegreesToRadians(315))
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_i == l2.index_i) return l1.index_j > l2.index_j;
            return l1.index_i < l2.index_i;
        });
    }
    else
    {
        std::sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
            if (l1.index_j == l2.index_j) return l1.index_i < l2.index_i;
            return l1.index_j < l2.index_j;
        });
    }
}

void FloatingHorizon::resetRotation()
{
    transformations = Matrix::identity();
    angleX = 0.0;
    angleY = 0.0;
    rotateView(transformations);
}

void FloatingHorizon::draw(QPainter& painter)
{
    resetHorizonts();
    for (const auto& line : lines)
        drawLine(painter, line);
}

void FloatingHorizon::updateLines()
{
    lines.clear();
    lines.reserve(viewport.nTicks * (2 * viewport.xCount * viewport.zCount - viewport.xCount - viewport.zCount));

    Vector start(viewport.xStart, viewport.yStart, viewport.zStart);
    Vector end(viewport.xEnd, viewport.yEnd, viewport.zEnd);

    auto dir = end - start;
    auto delta = end - start;
    auto center = dir / 2;
    delta.setX(delta.getX() / (viewport.xCount - 1));
    delta.setZ(delta.getZ() / (viewport.zCount - 1));

    auto projectorX = [=](double x) {
        auto f = (x - viewport.xStart) / dir.getX();
        return width * (f * SQRT1_2 + 0.5 * (1 - SQRT1_2));
    };
    auto projectorZ = [=](double z) {
        auto f = (z - start.getZ()) / dir.getZ();
        return width * (f * SQRT1_2 + 0.5 * (1 - SQRT1_2));
    };

    const size_t N = viewport.nTicks + 1;

    for (size_t i = 0; i < viewport.xCount; i++)
    {
        double x = viewport.xStart + delta.getX() * i;
        double xp = projectorX(x);
        for (size_t j = 0; j < viewport.zCount; j++)
        {
            double z = viewport.zStart + delta.getZ() * j;
            double zp = projectorZ(z);
            double y = function(x, z);

            if (i < viewport.xCount - 1)
            {
                double x1 = x;
                double xp1 = xp;
                double y1 = y;

                for (size_t k = 1; k <= viewport.nTicks; k++)
                {
                    double x2 = x + delta.getX() * k / viewport.nTicks;
                    double xp2 = projectorX(x2);
                    double y2 = function(x2, z);

                    lines.push_back(Line(Vector(xp1, y1, zp), Vector(xp2, y2, zp), i * N + k, j * N));

                    x1 = x2;
                    xp1 = xp2;
                    y1 = y2;
                }
            }
            if (j < viewport.zCount - 1)
            {
                double z1 = z;
                double zp1 = zp;
                double y1 = y;

                for (size_t k = 1; k <= viewport.nTicks; k++)
                {
                    double z2 = z + delta.getZ() * k / viewport.nTicks;
                    double zp2 = projectorZ(z2);
                    double y2 = function(x, z2);

                    lines.push_back(Line(Vector(xp, y1, zp1), Vector(xp, y2, zp2), i * N, j * N + k));

                    z1 = z2;
                    zp1 = zp2;
                    y1 = y2;
                }
            }
        }
    }
}

void FloatingHorizon::normalizeAngles()
{
    constexpr auto two_pi = qDegreesToRadians(360);

    while (angleX < 0.0) angleX += two_pi;
    while (angleX >= two_pi) angleX -= two_pi;

    while (angleY < 0.0) angleY += two_pi;
    while (angleY >= two_pi) angleY -= two_pi;
}

void FloatingHorizon::resetHorizonts()
{
    std::memset(upperHorizon, 10000, horizonSize * sizeof(int));
    std::memset(lowerHorizon, 0, horizonSize * sizeof(int));
}

void FloatingHorizon::drawLine(QPainter& painter, const Line& line)
{
    double deltaX = line.p2.getX() - line.p1.getX();

    if (deltaX <= step)
    {
        long i = line.p1.getX() / step;

        if (i >= 0 && i < width)
        {
            Line temp = line;
            if (temp.p2.getY() < temp.p1.getY())
                std::swap(temp.p1, temp.p2);

            double y_min = temp.p1.getY(), y_max = temp.p2.getY();
            double y_up = upperHorizon[i], y_bel = lowerHorizon[i];

            if (y_max <= y_up || y_bel <= y_min)
                painter.drawLine(temp.p1.getX(), y_min, temp.p2.getX(), y_max);
            else
            {
                if (y_min < y_up)
                    painter.drawLine(temp.p1.getX(), temp.p1.getY(), temp.p2.getX(), y_up);

                if (y_bel < y_max)
                    painter.drawLine(temp.p1.getX(), y_bel, temp.p2.getX(), temp.p2.getY());
            }
        }
        return;
    }

    double deltaY = line.p2.getY() - line.p1.getY();
    double m = deltaY / deltaX;

    double xPrev = line.p1.getX();
    double yPrev = line.p1.getY();

    long i = static_cast<long>(xPrev / step);
    bool prevUpper = (xPrev < 0.0 || xPrev >= width * step) ? false : yPrev <= upperHorizon[i];
    bool prevLower = (xPrev < 0.0 || xPrev >= width * step) ? false : yPrev >= lowerHorizon[i];

    for (double x = line.p1.getX() + step; x < line.p2.getX() + step; x += step)
    {
        double y = m * (x - line.p1.getX()) + line.p1.getY();
        i = x / step;

        if (i >= 0 && i < width)
        {
            bool upper = y <= upperHorizon[i];
            bool lower = y >= lowerHorizon[i];

            if (upper) upperHorizon[i] = y;
            if (lower) lowerHorizon[i] = y;

            if ((prevUpper || prevLower) && (upper || lower))
            {
                // painter.setPen(Qt::black);
                painter.drawLine(xPrev, yPrev, x, y);
            }

            prevUpper = upper;
            prevLower = lower;
        }
        else
        {
            prevUpper = false;
            prevLower = false;
        }

        xPrev = x;
        yPrev = y;
    }
}

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
