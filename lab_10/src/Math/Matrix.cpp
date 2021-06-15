#include <cstring>
#include <cmath>
#include "Matrix.hpp"

constexpr Matrix::Matrix()
    : data{ 0 }
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            data[i][j] = 0.0;
}

constexpr Matrix::Matrix(double mat[4][4])
    : data{ 0 }
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            data[i][j] = mat[i][j];
}

Matrix& Matrix::operator=(const Matrix& mat) noexcept
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            data[i][j] = mat.data[i][j];

    return *this;
}

constexpr double Matrix::det() const
{
    double det = 0.0;

    for (int i = 0; i < 4; i++)
        det += (data[0][i] * data[2][(i + 2) % 4] *
            (data[1][(i + 1) % 4] * data[3][(i + 3) % 4]
           - data[1][(i + 3) % 4] * data[3][(i + 1) % 4]));

    return det;
}

double Matrix::at(int row, int col) const
{
    return data[row][col];
}

void Matrix::translate(const Vector& offset)
{
    data[3][0] += offset.getX();
    data[3][1] += offset.getY();
    data[3][2] += offset.getZ();
}

void Matrix::rotate(const Vector& axis, double angle)
{
    *this = *this * Matrix::rotation(axis, angle);
}

void Matrix::scale(const Vector& factor)
{
    for (int i = 0; i < 4; i++)
    {
        data[i][0] *= factor.getX();
        data[i][1] *= factor.getY();
        data[i][2] *= factor.getZ();
    }
}

void Matrix::scale(double value)
{
    for (int i = 0; i < 4; i++)
    {
        data[i][0] *= value;
        data[i][1] *= value;
        data[i][2] *= value;
    }
}

Matrix Matrix::inverse() const
{
    Matrix res;

    res.data[0][0] =
        data[1][1] * data[2][2] * data[3][3] -
        data[1][1] * data[2][3] * data[3][2] -
        data[2][1] * data[1][2] * data[3][3] +
        data[2][1] * data[1][3] * data[3][2] +
        data[3][1] * data[1][2] * data[2][3] -
        data[3][1] * data[1][3] * data[2][2];

    res.data[1][0] = -
        data[1][0] * data[2][2] * data[3][3] +
        data[1][0] * data[2][3] * data[3][2] +
        data[2][0] * data[1][2] * data[3][3] -
        data[2][0] * data[1][3] * data[3][2] -
        data[3][0] * data[1][2] * data[2][3] +
        data[3][0] * data[1][3] * data[2][2];

    res.data[2][0] =
        data[1][0] * data[2][1] * data[3][3] -
        data[1][0] * data[2][3] * data[3][1] -
        data[2][0] * data[1][1] * data[3][3] +
        data[2][0] * data[1][3] * data[3][1] +
        data[3][0] * data[1][1] * data[2][3] -
        data[3][0] * data[1][3] * data[2][1];

    res.data[3][0] = -
        data[1][0] * data[2][1] * data[3][2] +
        data[1][0] * data[2][2] * data[3][1] +
        data[2][0] * data[1][1] * data[3][2] -
        data[2][0] * data[1][2] * data[3][1] -
        data[3][0] * data[1][1] * data[2][2] +
        data[3][0] * data[1][2] * data[2][1];

    res.data[0][1] = -
        data[0][1] * data[2][2] * data[3][3] +
        data[0][1] * data[2][3] * data[3][2] +
        data[2][1] * data[0][2] * data[3][3] -
        data[2][1] * data[0][3] * data[3][2] -
        data[3][1] * data[0][2] * data[2][3] +
        data[3][1] * data[0][3] * data[2][2];

    res.data[1][1] =
        data[0][0] * data[2][2] * data[3][3] -
        data[0][0] * data[2][3] * data[3][2] -
        data[2][0] * data[0][2] * data[3][3] +
        data[2][0] * data[0][3] * data[3][2] +
        data[3][0] * data[0][2] * data[2][3] -
        data[3][0] * data[0][3] * data[2][2];

    res.data[2][1] = -
        data[0][0] * data[2][1] * data[3][3] +
        data[0][0] * data[2][3] * data[3][1] +
        data[2][0] * data[0][1] * data[3][3] -
        data[2][0] * data[0][3] * data[3][1] -
        data[3][0] * data[0][1] * data[2][3] +
        data[3][0] * data[0][3] * data[2][1];

    res.data[3][1] =
        data[0][0] * data[2][1] * data[3][2] -
        data[0][0] * data[2][2] * data[3][1] -
        data[2][0] * data[0][1] * data[3][2] +
        data[2][0] * data[0][2] * data[3][1] +
        data[3][0] * data[0][1] * data[2][2] -
        data[3][0] * data[0][2] * data[2][1];

    res.data[0][2] =
        data[0][1] * data[1][2] * data[3][3] -
        data[0][1] * data[1][3] * data[3][2] -
        data[1][1] * data[0][2] * data[3][3] +
        data[1][1] * data[0][3] * data[3][2] +
        data[3][1] * data[0][2] * data[1][3] -
        data[3][1] * data[0][3] * data[1][2];

    res.data[1][2] = -
        data[0][0] * data[1][2] * data[3][3] +
        data[0][0] * data[1][3] * data[3][2] +
        data[1][0] * data[0][2] * data[3][3] -
        data[1][0] * data[0][3] * data[3][2] -
        data[3][0] * data[0][2] * data[1][3] +
        data[3][0] * data[0][3] * data[1][2];

    res.data[2][2] =
        data[0][0] * data[1][1] * data[3][3] -
        data[0][0] * data[1][3] * data[3][1] -
        data[1][0] * data[0][1] * data[3][3] +
        data[1][0] * data[0][3] * data[3][1] +
        data[3][0] * data[0][1] * data[1][3] -
        data[3][0] * data[0][3] * data[1][1];

    res.data[3][2] = -
        data[0][0] * data[1][1] * data[3][2] +
        data[0][0] * data[1][2] * data[3][1] +
        data[1][0] * data[0][1] * data[3][2] -
        data[1][0] * data[0][2] * data[3][1] -
        data[3][0] * data[0][1] * data[1][2] +
        data[3][0] * data[0][2] * data[1][1];

    res.data[0][3] = -
        data[0][1] * data[1][2] * data[2][3] +
        data[0][1] * data[1][3] * data[2][2] +
        data[1][1] * data[0][2] * data[2][3] -
        data[1][1] * data[0][3] * data[2][2] -
        data[2][1] * data[0][2] * data[1][3] +
        data[2][1] * data[0][3] * data[1][2];

    res.data[1][3] =
        data[0][0] * data[1][2] * data[2][3] -
        data[0][0] * data[1][3] * data[2][2] -
        data[1][0] * data[0][2] * data[2][3] +
        data[1][0] * data[0][3] * data[2][2] +
        data[2][0] * data[0][2] * data[1][3] -
        data[2][0] * data[0][3] * data[1][2];

    res.data[2][3] = -
        data[0][0] * data[1][1] * data[2][3] +
        data[0][0] * data[1][3] * data[2][1] +
        data[1][0] * data[0][1] * data[2][3] -
        data[1][0] * data[0][3] * data[2][1] -
        data[2][0] * data[0][1] * data[1][3] +
        data[2][0] * data[0][3] * data[1][1];

    res.data[3][3] =
        data[0][0] * data[1][1] * data[2][2] -
        data[0][0] * data[1][2] * data[2][1] -
        data[1][0] * data[0][1] * data[2][2] +
        data[1][0] * data[0][2] * data[2][1] +
        data[2][0] * data[0][1] * data[1][2] -
        data[2][0] * data[0][2] * data[1][1];

    return res;
}

Matrix Matrix::operator*(const Matrix& mat) const
{
    Matrix res;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                res.data[i][j] += data[i][k] * mat.data[k][j];

    return res;
}

Matrix Matrix::operator*(double value) const
{
    Matrix result = *this;
    result.scale(value);
    return result;
}

Matrix Matrix::zero()
{
    return Matrix();
}

Matrix Matrix::identity()
{
    Matrix mat;

    for (int i = 0; i < 4; i++)
        mat.data[i][i] = 1.0;

    return mat;
}

Matrix Matrix::translation(const Vector& offset)
{
    Matrix res;

    res.data[0][0] = 1.0;
    res.data[1][1] = 1.0;
    res.data[2][2] = 1.0;
    res.data[3][3] = 1.0;

    res.data[3][0] = offset.getX();
    res.data[3][1] = offset.getY();
    res.data[3][2] = offset.getY();

    return res;
}

Matrix Matrix::rotation(const Vector& axis, double angle)
{
    Matrix res;
    double len = axis.getLength();
    double x = axis.getX() / len, y = axis.getY() / len, z = axis.getZ() / len;
    double c = std::cos(angle), s = std::sin(angle);
    double imc = 1.0 - c;

    res.data[0][0] = c + x * x * imc;
    res.data[0][1] = x * y * imc + z * s;
    res.data[0][2] = x * z * imc - y * s;

    res.data[1][0] = x * y * imc - z * s;
    res.data[1][1] = c + y * y * imc;
    res.data[1][2] = y * z * imc + x * s;

    res.data[2][0] = x * z * imc + y * s;
    res.data[2][1] = y * z * imc - x * s;
    res.data[2][2] = c + z * z * imc;

    res.data[3][3] = 1.0;

    return res;
}

Matrix Matrix::scaling(const Vector& factor)
{
    Matrix res;

    res.data[0][0] = factor.getX();
    res.data[1][1] = factor.getY();
    res.data[2][2] = factor.getZ();
    res.data[3][3] = 1.0;

    return res;
}

Matrix Matrix::perspective(double fov, double aspect, double near, double far)
{
    Matrix res;

    double cot = 1 / std::tan(fov / 2);

    res.data[0][0] = cot / aspect;
    res.data[1][1] = cot;
    res.data[2][2] = (near + far) / (near - far);
    res.data[3][2] = -1;
    res.data[2][3] = 2 * near * far / (near - far);

    return res;
}

Vector operator*(const Vector& vec, const Matrix& mat)
{
    Vector res;

    res.setX(mat.data[0][0] * vec.getX() + mat.data[1][0] * vec.getY() + mat.data[2][0] * vec.getZ() + mat.data[3][0]);
    res.setY(mat.data[0][1] * vec.getX() + mat.data[1][1] * vec.getY() + mat.data[2][1] * vec.getZ() + mat.data[3][1]);
    res.setZ(mat.data[0][2] * vec.getX() + mat.data[1][2] * vec.getY() + mat.data[2][2] * vec.getZ() + mat.data[3][2]);

    return res;
}
