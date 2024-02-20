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

color_t diffuser_color(const Scene &scene, const ray_t &ray, const ray_collision &collision)
{
    color_t result_intensivity = {0, 0, 0};
    result_intensivity = result_intensivity + scene.AMBIENT_LIGHT;

    for (DirectionLight light : scene.directioned_lights)
    {
        float mul = scalarMultiplication(light.direction.normalized(), collision.intersection.normale.normalized());

        if (mul > 0)
            result_intensivity = result_intensivity + light.intensity * mul;
    }

    for (PointLight light : scene.pointed_lights)
    {
        color_t I = light.intensity;
        Point direction = light.position - collision.intersection.point;
        float R = direction.length();
        float U = light.attenuation.red + light.attenuation.green * R + light.attenuation.blue * R * R;
        color_t apllied_attenuation = I / U;
        float mul = scalarMultiplication(direction.normalized(), collision.intersection.normale.normalized());

        if (mul > 0)
            result_intensivity = result_intensivity + light.intensity * mul;
    }

    float red = collision.primitive->color.red * result_intensivity.red;
    float green = collision.primitive->color.green * result_intensivity.green;
    float blue = collision.primitive->color.blue * result_intensivity.blue;

    return { std::min(red, 1.f), std::min(green, 1.f), std::min(blue, 1.f) };
}

color_t ray_color(const Scene &scene, const ray_t &ray, float coeff_limit, int depth)
{
    ray_collision collision = first_intersection(scene, ray, coeff_limit);

    if (collision.intersection.success)
    {
        switch (collision.primitive->material)
        {
        case Material::METALLIC:
        case Material::DIELECTRIC:
        case Material::DIFFUSER:
            return diffuser_color(scene, ray, collision);

            break;
        default:
            std::cerr << "Unexpected material type " << collision.primitive->material;

            break;
        }
        return collision.primitive->color;
    }
    else
        return scene.BACKGROUND_COLOR;
}
