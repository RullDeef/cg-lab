#pragma once

#include "Math/Vector.hpp"


struct Line
{
    Vector p1;
    Vector p2;

    explicit Line(const Vector& p1, const Vector& p2) noexcept;
    Vector center() const noexcept;
    double z() const noexcept;
};
