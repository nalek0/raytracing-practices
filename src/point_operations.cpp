#include "data.hpp"

#include <cmath>

color_t color_t::operator*(const float k) const
{
    return {red * k, green * k, blue * k};
}

color_t color_t::operator/(const float k) const
{
    return {red / k, green / k, blue / k};
}

color_t color_t::operator+(const color_t &other) const
{
    return {red + other.red, green + other.green, blue + other.blue};
}

Point Point::operator+(const Point &right) const
{
    return {
        x + right.x,
        y + right.y,
        z + right.z};
}

Point Point::operator-(const Point &right) const
{
    return {
        x - right.x,
        y - right.y,
        z - right.z};
}

Point Point::operator-() const
{
    return {-x, -y, -z};
}

Point Point::operator*(const float k) const
{
    return {k * x, k * y, k * z};
}

Point Point::operator/(const float k) const
{
    return {x / k, y / k, z / k};
}

std::ostream &operator<<(std::ostream &os, const Point &p)
{
    return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

float Point::length() const
{
    return sqrt(x * x + y * y + z * z);
}

Point Point::normalized() const
{
    return Point(x, y, z) / this->length();
}

float scalarMultiplication(const Point &left, const Point &right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

Point vectorMultiplication(const Point &left, const Point &right)
{
    return Point(
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x);
}

Point componentDivision(const Point &left, const Point &right)
{
    return Point(left.x / right.x, left.y / right.y, left.z / right.z);
}

Quaternion Quaternion::conjugate() const
{
    Point new_p = Point(-p.x, -p.y, -p.z);

    return Quaternion(new_p, w);
}

Quaternion Quaternion::operator*(const Quaternion &other)
{
    Point othP = other.p;

    return Quaternion(
        othP * w + p * other.w + vectorMultiplication(p, othP),
        w * other.w - scalarMultiplication(p, othP));
}

Point rotate(const Point &p, const Quaternion &q)
{
    Quaternion quaternion = q;
    Quaternion first_mul = quaternion * Quaternion(p, 0);
    Quaternion second_mul = first_mul * q.conjugate();

    return second_mul.p;
}
