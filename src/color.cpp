#include "color.hpp"

Color Color::operator*(const float k) const
{
    return {red * k, green * k, blue * k};
}

Color Color::operator/(const float k) const
{
    return {red / k, green / k, blue / k};
}

Color Color::operator+(const Color &other) const
{
    return {red + other.red, green + other.green, blue + other.blue};
}

Color& Color::operator+=(const Color &other)
{
    red += other.red;
    green += other.green;
    blue += other.blue;

    return *this;
}
