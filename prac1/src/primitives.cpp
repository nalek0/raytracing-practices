#include <iostream>
#include <cmath>

#include "data.hpp"

Plane::Plane(Point _normal_direction) : normal_direction(_normal_direction) {}
Ellipsoid::Ellipsoid(float _rx, float _ry, float _rz) : rx(_rx), ry(_ry), rz(_rz) {}
Box::Box(float _sizex, float _sizey, float _sizez) : sizex(_sizex), sizey(_sizey), sizez(_sizez) {}

intersection_result_t Plane::intersect(const ray_t ray)
{
    Point ray_position = ray.position;
    Point ray_direction = ray.direction;
    float t = (scalar(center_position, normal_direction) - scalar(ray_position, normal_direction)) 
        / scalar(ray_direction, normal_direction);
    Point ttimes = ray_direction * t;
    Point result = ray_position + ttimes;

    return {t > 0, result};
}

intersection_result_t Ellipsoid::intersect(const ray_t ray)
{
    Point ray_position = ray.position;
    Point ray_direction = ray.direction;
    Point O = ray_position - center_position;
    Point D = ray_direction;
    Point R = Point(rx, ry, rz);
    Point OdivR = componentDivision(O, R);
    Point DdivR = componentDivision(D, R);
    float c = scalar(OdivR, OdivR) - 1;
    float b = 2 * scalar(OdivR, DdivR);
    float a = scalar(DdivR, DdivR);
    // c + bt + a t^2 = 0
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float t1 = (-b + sqrt(discriminant)) / (2 * a);
        float t2 = (-b - sqrt(discriminant)) / (2 * a);

        if (t2 > 0) {
            Point timed = D * t2;
            
            return {true, O + timed + center_position};
        } else if (t1 > 0) {
            Point timed = D * t1;

            return {true, O + timed + center_position};
        } else {
            return {false, Point()};
        }
    } else {
        return {false, Point()};
    }
    
    return { false, Point() }; // TODO
}

intersection_result_t Box::intersect(const ray_t ray)
{
    return {false, Point() }; // TODO
}
