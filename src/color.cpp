#include <cmath>
#include <fstream>

#include "color.hpp"

Color Color::operator+(const float k) const
{
    return {red + k, green + k, blue + k};
}

Color Color::operator*(const float k) const
{
    return {red * k, green * k, blue * k};
}

Color Color::operator*(const Color &other) const
{
    return {red * other.red, green * other.green, blue * other.blue};
}

Color Color::operator/(const float k) const
{
    return {red / k, green / k, blue / k};
}

Color Color::operator/(const Color& other) const
{
    return {red / other.red, green / other.green, blue / other.blue};
}

Color Color::operator+(const Color &other) const
{
    return {red + other.red, green + other.green, blue + other.blue};
}

Color &Color::operator+=(const Color &other)
{
    red += other.red;
    green += other.green;
    blue += other.blue;

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Color &col)
{
    return os << "Color["
              << "red = " << col.red
              << ", green = " << col.green
              << ", blue = " << col.blue << "]";
}

Color gamma_correction(const Color &color)
{
    const float alpha = 1 / 2.2;

    return {
        .red = std::pow(color.red, alpha),
        .green = std::pow(color.green, alpha),
        .blue = std::pow(color.blue, alpha)};
}

char get_red(const Color &color)
{
    return round(color.red * 255);
}

char get_green(const Color &color)
{
    return round(color.green * 255);
}

char get_blue(const Color &color)
{
    return round(color.blue * 255);
}

Color clamp(const Color &color, const Color &min_color, const Color &max_color)
{
    return {
        .red = std::min(std::max(color.red, min_color.red), max_color.red),
        .green = std::min(std::max(color.green, min_color.green), max_color.green),
        .blue = std::min(std::max(color.blue, min_color.blue), max_color.blue)};
}

Color saturate(const Color &color)
{
    return clamp(color, {0, 0, 0}, {1, 1, 1});
}

Color aces_tonemap(const Color &color)
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;

    return saturate((color * (color * a + b)) / (color * (color * c + d) + e));
}
