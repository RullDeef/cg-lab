#include <cmath>
#include "Transform.hpp"


Transform::Transform()
    : Matrix(Matrix::identity())
{
}

Transform::Transform(const Matrix& mat)
    : Matrix(mat)
{
}

Transform& Transform::operator=(const Transform& t)
{
    Matrix::operator=(t);
    return *this;
}

Transform& Transform::apply(const Transform& t)
{
    return *this = *this * t;
}

Vector Transform::getPosition() const
{
    return Vector(at(3, 0), at(3, 1), at(3, 2));
}

Vector Transform::getRotation() const
{
    Vector scale = getScale();

    double yaw = atan2(-at(2, 0) / scale.getX(), at(0, 0) / scale.getX());
    double pitch = asin(at(1, 0) / scale.getZ());
    double roll = atan2(-at(1, 2) / scale.getY(), at(1, 1) / scale.getY());

    return Vector(yaw, roll, pitch);
}

Vector Transform::getScale() const
{
    Vector result;

    result.setX(std::sqrt(at(0, 0) * at(0, 0) + at(0, 1) * at(0, 1) + at(0, 2) * at(0, 2)));
    result.setY(std::sqrt(at(1, 0) * at(1, 0) + at(1, 1) * at(1, 1) + at(1, 2) * at(1, 2)));
    result.setZ(std::sqrt(at(2, 0) * at(2, 0) + at(2, 1) * at(2, 1) + at(2, 2) * at(2, 2)));

    return result;
}
