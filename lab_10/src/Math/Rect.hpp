#pragma once

#include "Vector.hpp"

class Rect
{
public:
    constexpr Rect(double left = 0.0, double top = 0.0, double width = 0.0, double height = 0.0)
        : left(left), top(top), width(width), height(height) {}

    constexpr double getLeft() const    { return left; }
    constexpr double getTop() const     { return top; }
    constexpr double getBottom() const  { return top + height; }
    constexpr double getRight() const   { return left + width; }
    constexpr double getWidth() const   { return width; }
    constexpr double getHeight() const  { return height; }

    constexpr Vector getTopLeft() const { return Vector(left, top, 0.0); }
    constexpr Vector getSize() const    { return Vector(width, height, 0.0); }
    constexpr double getAspect() const  { return height / width; }

private:
    double left;
    double top;
    double width;
    double height;
};
