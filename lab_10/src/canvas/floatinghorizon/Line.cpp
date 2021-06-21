#include "Line.hpp"


Line::Line(const Vector& p1, const Vector& p2, size_t index_i, size_t index_j) noexcept
    : p1(p1), p2(p2), index_i(index_i), index_j(index_j)
{
}

Vector Line::center() const noexcept
{
    return (p1 + p2) * 0.5;
}
