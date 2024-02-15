#include <iostream>

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

    std::cout << "Points test:\n";
    point_t p = {1, 1, 1};
    point_t q = {-1, 1, -1};

    std::cout << (p + q) << ' ' << (p * q) << std::endl;

    return 0;
}
