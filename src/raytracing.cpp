#include <iostream>
#include <cmath>

#include "data.hpp"
#include "raytracing.hpp"

ray_t generate_ray(const Scene &scene, const int x, const int y)
{
    float px = (2. * (0.5 + x) / scene.WIDTH - 1) * tan(scene.FOV_X / 2);
    float py = -1 * (2. * (0.5 + y) / scene.HEIGHT - 1) * tan(scene.FOV_Y / 2);
    Point pixel_direction = scene.CAMERA_FORWARD.normalized() + scene.CAMERA_RIGHT.normalized() * px + scene.CAMERA_UP.normalized() * py;
    ray_t pixel_ray = {scene.CAMERA_POSITION, pixel_direction};

    return pixel_ray;
}

ray_collision_t first_intersection(const Scene &scene, const ray_t &ray, float coeff_limit)
{
    float ray_coeff = coeff_limit;
    Primitive *result_primitive = nullptr;
    intersection_result_t result_intersection = {.success = false};

    for (Primitive *primitive : scene.primitives)
    {
        intersection_result inter = primitive->intersect(ray, ray_coeff);

        if (inter.success)
        {
            result_primitive = primitive;
            result_intersection = inter;
            ray_coeff = inter.direction_coeff;
        }
    }

    return {
        .primitive = result_primitive,
        .intersection = result_intersection};
}

color_t ray_color(const Scene &scene, const ray_t &ray, float coeff_limit, int depth)
{
    ray_collision collision = first_intersection(scene, ray, coeff_limit);

    if (collision.intersection.success)
        return collision.primitive->color;
    else
        return scene.BACKGROUND_COLOR;
}
