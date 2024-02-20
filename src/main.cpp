#include <iostream>
#include <fstream>
#include <cmath>

#include "raytracing.hpp"
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
            color_t color = ray_color(scene, pixel_ray);

            char byte1 = ((char)round(color.red * 255));
            char byte2 = ((char)round(color.green * 255));
            char byte3 = ((char)round(color.blue * 255));
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
    
    // ray_t ray = generate_ray(scene, scene.WIDTH / 2, scene.HEIGHT / 2);
    // ray_collision collision = first_intersection(scene, ray, 1000);
    // color_t result = diffuser_color(scene, ray, collision);
    // std::cout << result.red << ' ' << result.green << ' ' << result.blue << std::endl;
    generate_image(scene, output_file);
    
    return 0;
}
