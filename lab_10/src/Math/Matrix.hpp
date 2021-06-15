#pragma once

#include "Vector.hpp"

// row major
class Matrix
{
public:
    constexpr Matrix();
    constexpr Matrix(double mat[4][4]);

    Matrix& operator=(const Matrix& mat) noexcept;

    constexpr double det() const;

    double at(int row, int col) const;

    void translate(const Vector& offset);
    void rotate(const Vector& axis, double angle);
    void scale(const Vector& factor);
    void scale(double value);
    Matrix inverse() const;

    Matrix operator*(const Matrix& mat) const;
    Matrix operator*(double value) const;

    friend Vector operator*(const Vector& vec, const Matrix& mat);

    static Matrix zero();
    static Matrix identity();

    static Matrix translation(const Vector& offset);
    static Matrix rotation(const Vector& axis, double angle);
    static Matrix scaling(const Vector& factor);

    static Matrix perspective(double fov, double aspect, double near, double far);

private:
    double data[4][4];
};
