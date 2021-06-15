#pragma once

#include "Matrix.hpp"


class Transform : public Matrix
{
public:
    Transform();
    Transform(const Matrix& mat);

    Transform& operator=(const Transform& t);
    Transform& apply(const Transform& t);

    Vector getPosition() const;
    Vector getRotation() const;
    Vector getScale() const;
};
