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

color_t ray_color(const Scene &scene, const ray_t &ray, float coeff_limit, int depth)
{
    float ray_length = 1000;
    color_t ray_color = scene.BACKGROUND_COLOR;
    for (int i = 0; i < scene.primitives.size(); i++)
    {
        Primitive *primitive = scene.primitives[i];
        intersection_result inter = primitive->intersect(ray, 1000);

        if (inter.success)
        {
            float new_length = (inter.point - scene.CAMERA_POSITION).length();

            if (new_length < ray_length)
            {
                ray_length = new_length;
                ray_color = primitive->color;
            }
        }
    }

    return ray_color;
}
