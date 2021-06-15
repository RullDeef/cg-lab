#include "Vector.hpp"
#include <cmath>

double Vector::getLength() const noexcept
{
    return hypot(hypot(x, y), z);
}

Vector operator*(double val, const Vector& v) noexcept
{
    return Vector(val * v.x, val * v.y, val * v.z);
}
