#include <iostream>
#include <fstream>
#include <cmath>

#include "data.hpp"

ray_t makePixelRay(const Scene &scene, const int x, const int y)
{
    float px = (2. * (0.5 + x) / scene.WIDTH - 1) * tan(scene.FOV_X / 2);
    float py = -1 * (2. * (0.5 + y) / scene.HEIGHT - 1) * tan(scene.FOV_Y / 2);
    Point pixel_direction = scene.CAMERA_FORWARD.normalized() + scene.CAMERA_RIGHT.normalized() * px + scene.CAMERA_UP.normalized() * py;
    ray_t pixel_ray = {scene.CAMERA_POSITION, pixel_direction};

    return pixel_ray;
}

color_t getRayColor(const Scene &scene, const ray_t &ray)
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

void generateImage(const Scene &scene)
{
    std::ofstream output_file("output.ppm");
    output_file << "P6\n";
    output_file << scene.WIDTH << ' ' << scene.HEIGHT << '\n';
    output_file << 255 << '\n';

    for (int y = 0; y < scene.HEIGHT; y++)
    {
        for (int x = 0; x < scene.WIDTH; x++)
        {
            ray_t pixel_ray = makePixelRay(scene, x, y);
            color_t ray_color = getRayColor(scene, pixel_ray);

            char byte1 = ((char)round(ray_color.red * 255));
            char byte2 = ((char)round(ray_color.green * 255));
            char byte3 = ((char)round(ray_color.blue * 255));
            output_file.write(&byte1, sizeof(char));
            output_file.write(&byte2, sizeof(char));
            output_file.write(&byte3, sizeof(char));
        }
    }
}

int main()
{
    std::string input_file = "input.txt";
    CommandScanner scanner = CommandScanner(input_file);
    std::vector<Command> result = std::vector<Command>();
    scanner.parse(result);
    SceneBuilder builder = SceneBuilder();

    for (Command cmd : result)
    {
        builder.acceptCommand(cmd);
    }

    Scene scene = builder.getScene();

    std::cout << "WIDTH = " << scene.WIDTH << "\n";
    std::cout << "HEIGHT = " << scene.HEIGHT << "\n";
    std::cout << "BACKGROUND_COLOR = "
              << scene.BACKGROUND_COLOR.red << ", "
              << scene.BACKGROUND_COLOR.green << ", "
              << scene.BACKGROUND_COLOR.blue << "\n";
    std::cout << "CAMERA_POSITION = "
              << scene.CAMERA_POSITION.x << ", "
              << scene.CAMERA_POSITION.y << ", "
              << scene.CAMERA_POSITION.z << "\n";
    std::cout << "CAMERA_RIGHT = "
              << scene.CAMERA_RIGHT.x << ", "
              << scene.CAMERA_RIGHT.y << ", "
              << scene.CAMERA_RIGHT.z << "\n";
    std::cout << "CAMERA_UP = "
              << scene.CAMERA_UP.x << ", "
              << scene.CAMERA_UP.y << ", "
              << scene.CAMERA_UP.z << "\n";
    std::cout << "CAMERA_FORWARD = "
              << scene.CAMERA_FORWARD.x << ", "
              << scene.CAMERA_FORWARD.y << ", "
              << scene.CAMERA_FORWARD.z << "\n";
    std::cout << "FOV_X = " << scene.FOV_X << "\n";
    std::cout << "FOV_Y = " << scene.FOV_Y << "\n";
    std::cout << "PRIMITIVES size = " << scene.primitives.size() << std::endl;

    generateImage(scene);
    // ray_t ray = makePixelRay(scene, scene.WIDTH / 2, scene.HEIGHT / 2);
    // color_t ray_color = getRayColor(scene, ray);

    // std::cout << "ray_color : " << ray_color.red << ' ' << ray_color.green << ' ' << ray_color.blue << std::endl;

    return 0;
}
