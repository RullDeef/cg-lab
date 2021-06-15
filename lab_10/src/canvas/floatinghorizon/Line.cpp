#include "Line.hpp"


Line::Line(const Vector& p1, const Vector& p2) noexcept
    : p1(p1), p2(p2)
{
}

Vector Line::center() const noexcept
{
    return (p1 + p2) * 0.5;
}

double Line::z() const noexcept
{
    return center().getZ();
}
