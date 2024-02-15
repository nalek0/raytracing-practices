#include "data.hpp"

#include <cmath>

Point Point::operator+(const Point &right)
{
    return {
        x + right.x,
        y + right.y,
        z + right.z};
}

Point Point::operator-(const Point &right)
{
    return {
        x - right.x,
        y - right.y,
        z - right.z};
}

Point Point::operator-()
{
    return {-x, -y, -z};
}

Point Point::operator*(const float k)
{
    return {k * x, k * y, k * z};
}

Point Point::operator/(const float k)
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

float scalar(const Point &left, const Point &right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}
