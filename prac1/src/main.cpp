#include <iostream>
#include <fstream>
#include <cmath>

#include "data.hpp"

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

    std::ofstream output_file("output.ppm");
    output_file << "P6\n";
    output_file << scene.WIDTH << ' ' << scene.HEIGHT << '\n';
    output_file << 255 << '\n';

    for (int x = 0; x < scene.WIDTH; x++)
    {
        for (int y = 0; y < scene.HEIGHT; y++)
        {
            color_t pixel_color = scene.BACKGROUND_COLOR;

            float px = (2 * (0.5 + x) / scene.WIDTH - 1) * tan(scene.FOV_X / 2);
            float py = -1 * (2 * (0.5 + y) / scene.HEIGHT - 1) * tan(scene.FOV_Y / 2);
            Point pixel_direction = scene.CAMERA_FORWARD.normalized() + scene.CAMERA_RIGHT.normalized() * px + scene.CAMERA_UP.normalized() * py;
            ray_t pixel_ray = {scene.CAMERA_POSITION, pixel_direction};
            intersection_result_t inter = scene.primitives[1]->intersect(pixel_ray);

            if (inter.success)
                pixel_color = scene.primitives[1]->color;

            char byte1 = ((char) round(pixel_color.red * 255));
            char byte2 = ((char) round(pixel_color.green * 255));
            char byte3 = ((char) round(pixel_color.blue * 255));
            output_file.write(&byte1, sizeof(char));
            output_file.write(&byte2, sizeof(char));
            output_file.write(&byte3, sizeof(char));
        }
    }

    return 0;
}
