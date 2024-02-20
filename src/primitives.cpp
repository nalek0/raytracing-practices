#include <iostream>
#include <cmath>

#include "scene.hpp"

Plane::Plane(Point _normal_direction) : normal_direction(_normal_direction) {}
Ellipsoid::Ellipsoid(float _rx, float _ry, float _rz) : rx(_rx), ry(_ry), rz(_rz) {}
Box::Box(float _sizex, float _sizey, float _sizez) : sizex(_sizex), sizey(_sizey), sizez(_sizez) {}

IntersectionResult Plane::intersect(const Ray ray, const float coeff_limit)
{
    Point O = rotate(ray.position - center_position, rotation.conjugate());
    Point D = rotate(ray.direction, rotation.conjugate());
    Point N = rotate(normal_direction, rotation.conjugate());
    float t = -scalarMultiplication(O, N) / scalarMultiplication(D, N);

    if (t > 0 && t < coeff_limit)
        return {
            .success = t > 0,
            .inside_primitive = false,
            .direction_coeff = t,
            .point = rotate(O + D * t, rotation) + center_position,
            .normale = rotate(normal_direction, rotation)};
    else
        return {.success = false};
}

IntersectionResult Ellipsoid::intersect(const Ray ray, const float coeff_limit)
{
    Point O = rotate(ray.position - center_position, rotation.conjugate());
    Point D = rotate(ray.direction, rotation.conjugate());
    Point R = Point(rx, ry, rz);
    Point OdivR = componentDivision(O, R);
    Point DdivR = componentDivision(D, R);
    float c = scalarMultiplication(OdivR, OdivR) - 1;
    float b = 2 * scalarMultiplication(OdivR, DdivR);
    float a = scalarMultiplication(DdivR, DdivR);
    // c + bt + a t^2 = 0
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0)
    {
        float t1 = (-b + sqrt(discriminant)) / (2 * a);
        float t2 = (-b - sqrt(discriminant)) / (2 * a);

        if (t2 > 0 && t2 < coeff_limit)
        {
            Point before_rotation_point = O + D * t2;
            Point before_rotation_normale = componentDivision(componentDivision(before_rotation_point, R), R);

            return {
                .success = true,
                .inside_primitive = false,
                .direction_coeff = t2,
                .point = rotate(before_rotation_point, rotation) + center_position,
                .normale = rotate(before_rotation_normale, rotation)};
        }
        else if (t1 > 0 && t1 < coeff_limit)
        {
            Point before_rotation_point = O + D * t1;
            Point before_rotation_normale = componentDivision(componentDivision(before_rotation_point, R), R);

            return {
                .success = true,
                .inside_primitive = true,
                .direction_coeff = t1,
                .point = rotate(before_rotation_point, rotation) + center_position,
                .normale = rotate(before_rotation_normale, rotation)};
        }
        else
        {
            return {.success = false};
        }
    }
    else
        return {.success = false};
}

Point Box::getNormale(Point point) const
{
    if (fabs(fabs(point.x) - sizex) < 1e-6)
        return Point(point.x, 0, 0);
    else if (fabs(fabs(point.y) - sizey) < 1e-6)
        return Point(0, point.y, 0);
    else
        return Point(0, 0, point.x);
}

IntersectionResult Box::intersect(const Ray ray, const float coeff_limit)
{
    Point O = rotate(ray.position - center_position, rotation.conjugate());
    Point D = rotate(ray.direction, rotation.conjugate());
    float t1x = (-sizex - O.x) / D.x;
    float t2x = (sizex - O.x) / D.x;
    float t1y = (-sizey - O.y) / D.y;
    float t2y = (sizey - O.y) / D.y;
    float t1z = (-sizez - O.z) / D.z;
    float t2z = (sizez - O.z) / D.z;

    if (t1x > t2x)
        std::swap(t1x, t2x);
    if (t1y > t2y)
        std::swap(t1y, t2y);
    if (t1z > t2z)
        std::swap(t1z, t2z);

    float t1 = std::max(std::max(t1x, t1y), t1z);
    float t2 = std::min(std::min(t2x, t2y), t2z);

    if (t1 > t2)
    {
        return {.success = false};
    }
    else if (t1 > 0 && t1 < coeff_limit)
    {
        Point before_rotation_point = O + D * t1;
        Point before_rotation_normale = getNormale(before_rotation_point);

        return {
            .success = true,
            .inside_primitive = false,
            .direction_coeff = t1,
            .point = rotate(before_rotation_point, rotation) + center_position,
            .normale = rotate(before_rotation_normale, rotation)};
    }
    else if (t2 > 0 && t2 < coeff_limit)
    {
        Point before_rotation_point = O + D * t2;
        Point before_rotation_normale = getNormale(before_rotation_point);

        return {
            .success = true,
            .inside_primitive = true,
            .direction_coeff = t2,
            .point = rotate(before_rotation_point, rotation) + center_position,
            .normale = rotate(before_rotation_normale, rotation)};
    }
    else
    {
        return {.success = false};
    }
}
