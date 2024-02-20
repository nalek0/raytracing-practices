#include "geometry.hpp"
#include "scene.hpp"

typedef struct intersection_result IntersectionResult;
typedef struct ray_collision RayCollision;

struct intersection_result
{
    bool success;

    // if success:
    bool inside_primitive;
    float direction_coeff;
    Point point;
    Point normale;
};

struct ray_collision
{
    Primitive *primitive;
    IntersectionResult intersection;
};

Ray generate_ray(const Scene &scene, const int x, const int y);
RayCollision first_intersection(const Scene &scene, const Ray &ray, float coeff_limit);
color_t diffuser_color(const Scene &scene, const Ray &ray, const ray_collision &collision);
color_t ray_color(const Scene &scene, const Ray &ray, float coeff_limit = 1000, int depth = 0);
