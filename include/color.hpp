#pragma once

typedef struct color color_t;

struct color
{
    float red;
    float green;
    float blue;

    color_t operator*(const float k) const;
    color_t operator/(const float k) const;
    color_t operator+(const color_t &other) const;
};
