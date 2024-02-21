#pragma once

#include <fstream>

typedef struct ray Ray;

class Point
{
private:
public:
    float x;
    float y;
    float z;
    Point() : x(0), y(0), z(0) {}
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float length() const;
    Point normalized() const;

    Point operator+(const Point &right) const;
    Point operator-(const Point &right) const;
    Point operator-() const;
    Point operator*(const float k) const;
    Point operator/(const float k) const;
    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

class Quaternion
{
public:
    Point p;
    float w;

    Quaternion() : p(Point()), w(1) {}
    Quaternion(Point _p, float _w) : p(_p), w(_w) {}

    Quaternion conjugate() const;
    Quaternion operator*(const Quaternion &other);
};

struct ray
{
    Point position;
    Point direction;
};

float scalarMultiplication(const Point &left, const Point &right);
Point vectorMultiplication(const Point &left, const Point &right);
Point componentDivision(const Point &left, const Point &right);
Point rotate(const Point &p, const Quaternion &q);
Point get_reflection(const Point &direction, const Point &normale);
Point get_refraction(const Point &direction, const Point &normale, const float ior1, const float ior2);
