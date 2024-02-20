#include "color.hpp"

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
