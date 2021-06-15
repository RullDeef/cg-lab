#pragma once

class Vector
{
public:
    constexpr Vector(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z) {}

    constexpr Vector(double vec[3])
        : x(vec[0]), y(vec[1]), z(vec[2]) {}

    constexpr double getX() const noexcept { return x; }
    constexpr double getY() const noexcept { return y; }
    constexpr double getZ() const noexcept { return z; }

    constexpr void setX(double val) noexcept { x = val; }
    constexpr void setY(double val) noexcept { y = val; }
    constexpr void setZ(double val) noexcept { z = val; }

    constexpr Vector operator-() const noexcept { return Vector(-x, -y, -z); }
    constexpr Vector operator+(const Vector& v) const noexcept { return Vector(x + v.x, y + v.y, z + v.z); }
    constexpr Vector operator-(const Vector& v) const noexcept { return Vector(x - v.x, y - v.y, z - v.z); }
    constexpr Vector operator*(double val) const noexcept { return Vector(x * val, y * val, z * val); }
    constexpr Vector operator/(double val) const noexcept { return Vector(x / val, y / val, z / val); }
    friend Vector operator*(double val, const Vector& v) noexcept;

    double getLength() const noexcept;

    constexpr static Vector zero()      { return Vector( 0.0,  0.0,  0.0); }
    constexpr static Vector up()        { return Vector( 0.0,  1.0,  0.0); }
    constexpr static Vector down()      { return Vector( 0.0, -1.0,  0.0); }
    constexpr static Vector left()      { return Vector(-1.0,  0.0,  0.0); }
    constexpr static Vector right()     { return Vector( 1.0,  0.0,  0.0); }
    constexpr static Vector forward()   { return Vector( 0.0,  0.0, -1.0); }
    constexpr static Vector backward()  { return Vector( 0.0,  0.0,  1.0); }

private:
    double x;
    double y;
    double z;
};
