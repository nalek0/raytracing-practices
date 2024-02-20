#include <iostream>
#include <fstream>
#include <cmath>

#include "data.hpp"

Scene parse_scene(const std::string &input_file)
{
    CommandScanner scanner = CommandScanner(input_file);
    std::vector<Command> result = std::vector<Command>();
    scanner.parse(result);
    SceneBuilder builder = SceneBuilder();

    for (Command cmd : result)
        builder.acceptCommand(cmd);

    Scene scene = builder.getScene();

    return scene;
}

ray_t generate_ray(const Scene &scene, const int x, const int y)
{
    float px = (2. * (0.5 + x) / scene.WIDTH - 1) * tan(scene.FOV_X / 2);
    float py = -1 * (2. * (0.5 + y) / scene.HEIGHT - 1) * tan(scene.FOV_Y / 2);
    Point pixel_direction = scene.CAMERA_FORWARD.normalized() + scene.CAMERA_RIGHT.normalized() * px + scene.CAMERA_UP.normalized() * py;
    ray_t pixel_ray = {scene.CAMERA_POSITION, pixel_direction};

    return pixel_ray;
}

color_t get_ray_color(const Scene &scene, const ray_t &ray)
{
    float ray_length = 1000;
    color_t ray_color = scene.BACKGROUND_COLOR;
    for (int i = 0; i < scene.primitives.size(); i++)
    {
        Primitive *primitive = scene.primitives[i];
        intersection_result inter = primitive->intersect(ray);

        if (inter.success)
        {
            float new_length = (inter.result - scene.CAMERA_POSITION).length();

            if (new_length < ray_length)
            {
                ray_length = new_length;
                ray_color = primitive->color;
            }
        }
    }

    return ray_color;
}

void generate_image(const Scene &scene, const std::string &output_file)
{
    std::ofstream file(output_file);
    file << "P6\n";
    file << scene.WIDTH << ' ' << scene.HEIGHT << '\n';
    file << 255 << '\n';

    for (int y = 0; y < scene.HEIGHT; y++)
    {
        for (int x = 0; x < scene.WIDTH; x++)
        {
            ray_t pixel_ray = generate_ray(scene, x, y);
            color_t ray_color = get_ray_color(scene, pixel_ray);

            char byte1 = ((char)round(ray_color.red * 255));
            char byte2 = ((char)round(ray_color.green * 255));
            char byte3 = ((char)round(ray_color.blue * 255));
            file.write(&byte1, sizeof(char));
            file.write(&byte2, sizeof(char));
            file.write(&byte3, sizeof(char));
        }
    }
}

int main(int argc, char **argv)
{
    std::string input_file = std::string(argv[1]);
    std::string output_file = std::string(argv[2]);
    Scene scene = parse_scene(input_file);
    generate_image(scene, output_file);
    
    for (Primitive * primitive : scene.primitives) {
        delete primitive;
    }

    return 0;
}
