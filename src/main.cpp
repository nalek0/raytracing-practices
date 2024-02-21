#include <iostream>
#include <fstream>
#include <cmath>

#include "scene.hpp"

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
            Ray pixel_ray = generate_ray(scene, x, y);
            Color color = ray_color(scene, pixel_ray);
            Color with_aces = aces_tonemap(color);
            Color corrected = gamma_correction(with_aces);

            char byte1 = get_red(corrected);
            char byte2 = get_green(corrected);
            char byte3 = get_blue(corrected);
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
    
    // Ray ray = generate_ray(scene, scene.WIDTH / 2, scene.HEIGHT / 2 + 1000);
    // RayCollision collision = first_intersection(scene, ray, 1000);
    // Color result = diffuser_color(scene, ray, collision);
    // std::cout << result.red << ' ' << result.green << ' ' << result.blue << std::endl;
    generate_image(scene, output_file);
    
    return 0;
}
