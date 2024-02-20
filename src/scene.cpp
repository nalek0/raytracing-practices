#include <iostream>
#include <vector>
#include <cmath>

#include "data.hpp"

Scene::Scene()
{
    this->primitives = std::vector<Primitive *>();
}

void Scene::checkData()
{
    FOV_Y = std::atan(std::tan(FOV_X / 2) * HEIGHT / WIDTH) * 2;
}

SceneBuilder::SceneBuilder()
{
    this->scene = Scene();
    this->current_primitive = nullptr;
    this->is_primitive_building = false;
}

void SceneBuilder::acceptCommand(const Command &command)
{
    if (command.getCommandName() == "DIMENSIONS")
    {
        scene.WIDTH = std::stoi(command.getArgs().at(0));
        scene.HEIGHT = std::stoi(command.getArgs().at(1));
    }
    else if (command.getCommandName() == "BG_COLOR")
    {
        scene.BACKGROUND_COLOR.red = std::stof(command.getArgs().at(0));
        scene.BACKGROUND_COLOR.green = std::stof(command.getArgs().at(1));
        scene.BACKGROUND_COLOR.blue = std::stof(command.getArgs().at(2));
    }
    else if (command.getCommandName() == "LIGHT_DIR")
    {
        // TO BE CONTINUED...
    }
    else if (command.getCommandName() == "CAMERA_POSITION")
    {
        scene.CAMERA_POSITION.x = std::stof(command.getArgs().at(0));
        scene.CAMERA_POSITION.y = std::stof(command.getArgs().at(1));
        scene.CAMERA_POSITION.z = std::stof(command.getArgs().at(2));
    }
    else if (command.getCommandName() == "CAMERA_RIGHT")
    {
        scene.CAMERA_RIGHT.x = std::stof(command.getArgs().at(0));
        scene.CAMERA_RIGHT.y = std::stof(command.getArgs().at(1));
        scene.CAMERA_RIGHT.z = std::stof(command.getArgs().at(2));
    }
    else if (command.getCommandName() == "CAMERA_UP")
    {
        scene.CAMERA_UP.x = std::stof(command.getArgs().at(0));
        scene.CAMERA_UP.y = std::stof(command.getArgs().at(1));
        scene.CAMERA_UP.z = std::stof(command.getArgs().at(2));
    }
    else if (command.getCommandName() == "CAMERA_FORWARD")
    {
        scene.CAMERA_FORWARD.x = std::stof(command.getArgs().at(0));
        scene.CAMERA_FORWARD.y = std::stof(command.getArgs().at(1));
        scene.CAMERA_FORWARD.z = std::stof(command.getArgs().at(2));
    }
    else if (command.getCommandName() == "CAMERA_FOV_X")
    {
        scene.FOV_X = std::stof(command.getArgs().at(0));
    }
    else if (command.getCommandName() == "NEW_PRIMITIVE")
    {
        if (is_primitive_building)
        {
            scene.primitives.push_back(current_primitive);
            current_primitive = nullptr;
        }

        is_primitive_building = true;
    }
    else if (command.getCommandName() == "PLANE")
    {
        Point normal_direction = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
        current_primitive = new Plane(normal_direction);
    }
    else if (command.getCommandName() == "ELLIPSOID")
    {
        current_primitive = new Ellipsoid(
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2)));
    }
    else if (command.getCommandName() == "BOX")
    {   
        current_primitive = new Box(
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2)));
    }
    else if (command.getCommandName() == "POSITION")
    {
        current_primitive->center_position = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
    }
    else if (command.getCommandName() == "ROTATION")
    {
        current_primitive->rotation = Quaternion(
            Point(
                std::stof(command.getArgs().at(0)),
                std::stof(command.getArgs().at(1)),
                std::stof(command.getArgs().at(2))),
            std::stof(command.getArgs().at(3)));
    }
    else if (command.getCommandName() == "COLOR")
    {
        current_primitive->color = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
    }
    else if (command.getCommandName() == "EOF")
    {
        if (is_primitive_building)
        {
            scene.primitives.push_back(current_primitive);
            is_primitive_building = false;
        }

        scene.checkData();
    }
    else
    {
        std::cerr << "WARNING: command " << command.getCommandName() << " not found" << std::endl;
        std::exit(1);
    }
}

Scene SceneBuilder::getScene()
{
    return scene;
}
