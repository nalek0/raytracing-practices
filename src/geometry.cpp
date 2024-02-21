#include "geometry.hpp"

#include <cmath>

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

Point get_reflection(const Point &direction, const Point &normale)
{
    Point D = direction.normalized();
    Point N = normale.normalized();
    Point R = D - N * 2 * scalarMultiplication(N, D);

    return R;
}

Point get_refraction(const Point &direction, const Point &normale, const float ior1, const float ior2)
{
    Point D = direction.normalized();
    Point N = normale.normalized();
    float ior_div = ior1 / ior2;
    float cos1 = scalarMultiplication(-D, N);
    float sin2 = ior_div * sqrt(1 - cos1 * cos1);
    float cos2 = sqrt(1 - sin2 * sin2);
    Point R = D * ior_div + N * (ior_div * cos1 - cos2);

    return R;
}
