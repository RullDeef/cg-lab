#pragma once


class Viewport
{
public:
    Viewport() = default;
    Viewport(double xStart, double xEnd, double zStart, double zEnd)
        : xStart(xStart), xEnd(xEnd), zStart(zStart), zEnd(zEnd) {}

    double xStart = -1.0;
    double xEnd = 1.0;
    double yStart = -1.0;
    double yEnd = 1.0;
    double zStart = -1.0;
    double zEnd = 1.0;

    size_t xCount = 10;
    size_t zCount = 10;
    size_t nTicks = 4;
};
