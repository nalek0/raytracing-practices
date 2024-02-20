#pragma once

#include <vector>

#include "color.hpp"
#include "parse.hpp"
#include "geometry.hpp"

enum Material
{
    METALLIC,
    DIELECTRIC,
    DIFFUSER
};

class Primitive
{
public:
    Point center_position;
    Quaternion rotation;
    color_t color;
    Material material = Material::DIFFUSER;

    virtual struct intersection_result intersect(const Ray ray, const float coeff_limit) = 0;
};

class Plane : public Primitive
{
private:
    Point normal_direction;

public:
    Plane(Point _normal_direction);
    struct intersection_result intersect(const Ray ray, const float coeff_limit);
};

class Ellipsoid : public Primitive
{
private:
    float rx;
    float ry;
    float rz;

public:
    Ellipsoid(float _rx, float _ry, float _rz);
    struct intersection_result intersect(const Ray ray, const float coeff_limit);
};

class Box : public Primitive
{
private:
    float sizex;
    float sizey;
    float sizez;

public:
    Box(float _sizex, float _sizey, float _sizez);

    struct intersection_result intersect(const Ray ray, const float coeff_limit);
    Point getNormale(Point point) const;
};

class PointLight
{
public:
    color_t intensity;
    Point position;
    color_t attenuation;

    PointLight(color_t _intensity, Point _position, color_t _attenuation) : intensity(_intensity), position(_position), attenuation(_attenuation) {}
};

class DirectionLight
{
public:
    color_t intensity;
    Point direction;

    DirectionLight(color_t _intensity, Point _direction) : intensity(_intensity), direction(_direction) {}
};

class Scene
{
public:
    int WIDTH;
    int HEIGHT;
    color_t BACKGROUND_COLOR;
    Point CAMERA_POSITION;
    Point CAMERA_RIGHT;
    Point CAMERA_UP;
    Point CAMERA_FORWARD;
    float FOV_X;
    float FOV_Y;
    int RAY_DEPTH;
    color_t AMBIENT_LIGHT;
    std::vector<PointLight> pointed_lights;
    std::vector<DirectionLight> directioned_lights;
    std::vector<Primitive *> primitives;

    Scene();
    void checkData();
};

class SceneBuilder
{
private:
    Scene scene;
    Primitive *current_primitive;
    bool is_primitive_building;

    // Light building params:
    bool light_building;
    bool light_pointed;
    bool light_directioned;
    color_t intensity;
    Point direction;
    Point position;
    color_t attenuation;

public:
    SceneBuilder();
    void acceptCommand(const Command &command);
    Scene getScene();
};
