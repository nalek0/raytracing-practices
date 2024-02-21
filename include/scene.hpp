#pragma once

#include <vector>

#include "color.hpp"
#include "parse.hpp"
#include "geometry.hpp"

typedef struct intersection_result IntersectionResult;
typedef struct ray_collision RayCollision;

struct intersection_result
{
    bool success;
    bool inside_primitive;
    float direction_coeff;
    Point point;
    Point normale;
};

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
    Color color;
    Material material = Material::DIFFUSER;

    virtual IntersectionResult intersect(const Ray ray, const float coeff_limit) = 0;
};

class Plane : public Primitive
{
private:
    Point normal_direction;

public:
    Plane(Point _normal_direction);
    IntersectionResult intersect(const Ray ray, const float coeff_limit);
};

class Ellipsoid : public Primitive
{
private:
    float rx;
    float ry;
    float rz;

public:
    Ellipsoid(float _rx, float _ry, float _rz);
    IntersectionResult intersect(const Ray ray, const float coeff_limit);
};

class Box : public Primitive
{
private:
    float sizex;
    float sizey;
    float sizez;

public:
    Box(float _sizex, float _sizey, float _sizez);

    IntersectionResult intersect(const Ray ray, const float coeff_limit);
    Point getNormale(Point point) const;
};

class PointLight
{
public:
    Intensity intensity;
    Point position;
    Attenuation attenuation;

    PointLight(Intensity _intensity, Point _position, Attenuation _attenuation) : intensity(_intensity), position(_position), attenuation(_attenuation) {}
};

class DirectionLight
{
public:
    Intensity intensity;
    Point direction;

    DirectionLight(Intensity _intensity, Point _direction) : intensity(_intensity), direction(_direction) {}
};

class Scene
{
public:
    int WIDTH;
    int HEIGHT;
    Color BACKGROUND_COLOR;
    Point CAMERA_POSITION;
    Point CAMERA_RIGHT;
    Point CAMERA_UP;
    Point CAMERA_FORWARD;
    float FOV_X;
    float FOV_Y;
    int RAY_DEPTH;
    Color AMBIENT_LIGHT;
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
    Color intensity;
    Point direction;
    Point position;
    Color attenuation;

public:
    SceneBuilder();
    void acceptCommand(const Command &command);
    Scene getScene();
};

struct ray_collision
{
    Primitive *primitive;
    IntersectionResult intersection;
};

Ray generate_ray(const Scene &scene, const int x, const int y);
RayCollision first_intersection(const Scene &scene, const Ray &ray, float coeff_limit);
Intensity apply_attenuation(const PointLight &light, const Point &point);
Color diffuser_color(const Scene &scene, const Ray &ray, const RayCollision &collision);
Color metallic_color(const Scene &scene, const Ray &ray, const RayCollision &collision, int depth);
Color ray_color(const Scene &scene, const Ray &ray, float coeff_limit = 1000, int depth = 0);
