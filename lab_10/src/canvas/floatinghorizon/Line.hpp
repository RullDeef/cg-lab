#pragma once

#include "Math/Vector.hpp"


struct Line
{
    Vector p1;
    Vector p2;

    size_t index_i;
    size_t index_j;

    Line(const Vector& p1, const Vector& p2, size_t index_i, size_t index_j) noexcept;
    Vector center() const noexcept;
};
