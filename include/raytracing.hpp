#include "data.hpp"

ray_t generate_ray(const Scene &scene, const int x, const int y);
ray_collision_t first_intersection(const Scene &scene, const ray_t &ray, float coeff_limit);
color_t diffuser_color(const Scene &scene, const ray_t &ray, const ray_collision &collision);
color_t ray_color(const Scene &scene, const ray_t &ray, float coeff_limit = 1000, int depth = 0);
