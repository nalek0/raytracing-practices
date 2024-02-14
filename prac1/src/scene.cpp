#include <iostream>
#include <vector>
#include <cmath>

#include "data.hpp"

Plane::Plane(point_t _normal_direction) : normal_direction(_normal_direction) {}

point_t Plane::intersect(const ray_t ray) {
    return { 0, 0, 0 }; // TODO
}

Ellipsoid::Ellipsoid(float _rx, float _ry, float _rz) : rx(_rx), ry(_ry), rz(_rz) {}

point_t Ellipsoid::intersect(const ray_t ray) {
    return { 0, 0, 0 }; // TODO
}

Box::Box(float _sizex, float _sizey, float _sizez) : sizex(_sizex), sizey(_sizey), sizez(_sizez) {}

point_t Box::intersect(const ray_t ray) {
    return { 0, 0, 0 }; // TODO
}

Scene::Scene()
{
    this->primitives = std::vector<Primitive*>();
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
        scene.WIDTH = command.getArgs().at(0);
        scene.HEIGHT = command.getArgs().at(1);
    }
    else if (command.getCommandName() == "BG_COLOR")
    {
        scene.BACKGROUND_COLOR.red = command.getArgs().at(0) * 255;
        scene.BACKGROUND_COLOR.green = command.getArgs().at(1) * 255;
        scene.BACKGROUND_COLOR.blue = command.getArgs().at(2) * 255;
    }
    else if (command.getCommandName() == "CAMERA_POSITION")
    {
        scene.CAMERA_POSITION.x = command.getArgs().at(0);
        scene.CAMERA_POSITION.y = command.getArgs().at(1);
        scene.CAMERA_POSITION.z = command.getArgs().at(2);
    }
    else if (command.getCommandName() == "CAMERA_RIGHT")
    {
        scene.CAMERA_RIGHT.x = command.getArgs().at(0);
        scene.CAMERA_RIGHT.y = command.getArgs().at(1);
        scene.CAMERA_RIGHT.z = command.getArgs().at(2);
    }
    else if (command.getCommandName() == "CAMERA_UP")
    {
        scene.CAMERA_UP.x = command.getArgs().at(0);
        scene.CAMERA_UP.y = command.getArgs().at(1);
        scene.CAMERA_UP.z = command.getArgs().at(2);
    }
    else if (command.getCommandName() == "CAMERA_FORWARD")
    {
        scene.CAMERA_FORWARD.x = command.getArgs().at(0);
        scene.CAMERA_FORWARD.y = command.getArgs().at(1);
        scene.CAMERA_FORWARD.z = command.getArgs().at(2);
    }
    else if (command.getCommandName() == "CAMERA_FOV_X")
    {
        scene.FOV_X = command.getArgs().at(0);
    }
    else
    {
        // Primitive editing

        if (command.getCommandName() == "NEW_PRIMITIVE")
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
            point_t normal_direction = {
                command.getArgs().at(0),
                command.getArgs().at(1),
                command.getArgs().at(2)};
            current_primitive = new Plane(normal_direction);
        }
        else if (command.getCommandName() == "ELLIPSOID")
        {
            current_primitive = new Ellipsoid(
                command.getArgs().at(0),
                command.getArgs().at(1),
                command.getArgs().at(2));
        }
        else if (command.getCommandName() == "BOX")
        {
            current_primitive = new Box(
                command.getArgs().at(0),
                command.getArgs().at(1),
                command.getArgs().at(2));
        }
        else if (command.getCommandName() == "POSITION")
        {
            current_primitive->center_position = {
                command.getArgs().at(0),
                command.getArgs().at(1),
                command.getArgs().at(2)};
        }
        else if (command.getCommandName() == "ROTATION")
        {
            current_primitive->rotation = {
                command.getArgs().at(0),
                command.getArgs().at(1),
                command.getArgs().at(2),
                command.getArgs().at(3)};
        }
        else if (command.getCommandName() == "COLOR")
        {
            current_primitive->color = {
                (uint8_t) command.getArgs().at(0),
                (uint8_t) command.getArgs().at(1),
                (uint8_t) command.getArgs().at(2)};
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
}

Scene SceneBuilder::getScene()
{
    return scene;
}
