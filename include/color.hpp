#pragma once

typedef struct color Color;
typedef struct color Intensity;
typedef struct color Attenuation;

struct color
{
    float red;
    float green;
    float blue;

    Color operator*(const float k) const;
    Color operator/(const float k) const;
    Color operator+(const Color &other) const;
    Color& operator+=(const Color &other);
};
