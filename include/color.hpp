#pragma once

#include <fstream>

typedef struct color Color;
typedef struct color Intensity;
typedef struct color Attenuation;

struct color
{
    float red;
    float green;
    float blue;

    Color operator+(const float k) const;
    Color operator*(const float k) const;
    Color operator*(const Color& other) const;
    Color operator/(const float k) const;
    Color operator/(const Color& other) const;
    Color operator+(const Color &other) const;
    Color &operator+=(const Color &other);
    friend std::ostream &operator<<(std::ostream &os, const struct color &col);
};

Color gamma_correction(const Color& color);
char get_red(const Color& color);
char get_green(const Color& color);
char get_blue(const Color& color);
Color clamp(const Color &color, const Color &min_color, const Color &max_color);
Color saturate(const Color &color);
Color aces_tonemap(const Color &color);
