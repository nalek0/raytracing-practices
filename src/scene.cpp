#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

#include "data.hpp"

Scene::Scene()
{
    this->primitives = std::vector<Primitive *>();
    this->directioned_lights = std::vector<DirectionLight>();
    this->pointed_lights = std::vector<PointLight>();
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
    this->light_building = false;
    this->light_pointed = false;
    this->light_directioned = false;
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
    else if (command.getCommandName() == "RAY_DEPTH")
    {
        scene.RAY_DEPTH = std::stoi(command.getArgs().at(0));
    }
    else if (command.getCommandName() == "AMBIENT_LIGHT")
    {
        scene.AMBIENT_LIGHT = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
    }
    else if (command.getCommandName() == "NEW_LIGHT")
    {
        if (light_building && light_pointed)
            scene.pointed_lights.push_back(PointLight(intensity, position, attenuation));
        else if (light_building && light_directioned)
            scene.directioned_lights.push_back(DirectionLight(intensity, direction));

        this->light_building = true;
        this->light_pointed = false;
        this->light_directioned = false;
    }
    else if (command.getCommandName() == "LIGHT_INTENSITY")
    {
        assert(light_building);

        this->intensity = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
    }
    else if (command.getCommandName() == "LIGHT_ATTENUATION")
    {
        assert(light_building && !light_directioned);

        this->light_pointed = true;
        this->attenuation = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
    }
    else if (command.getCommandName() == "LIGHT_DIRECTION")
    {
        assert(light_building && !light_pointed);

        this->light_directioned = true;
        this->direction = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
    }
    else if (command.getCommandName() == "LIGHT_POSITION")
    {
        assert(light_building && !light_directioned);

        this->light_pointed = true;
        this->position = {
            std::stof(command.getArgs().at(0)),
            std::stof(command.getArgs().at(1)),
            std::stof(command.getArgs().at(2))};
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
    else if (command.getCommandName() == "METALLIC")
    {
        current_primitive->material = Material::METALLIC;
    }
    else if (command.getCommandName() == "DIELECTRIC")
    {
        current_primitive->material = Material::DIELECTRIC;
    }
    else if (command.getCommandName() == "EOF")
    {
        if (is_primitive_building)
            scene.primitives.push_back(current_primitive);
        if (light_building && light_pointed)
            scene.pointed_lights.push_back(PointLight(intensity, position, attenuation));
        else if (light_building && light_directioned)
            scene.directioned_lights.push_back(DirectionLight(intensity, direction));

        scene.checkData();
    }
    else
    {
        std::cerr << "WARNING: command " << command.getCommandName() << " not found" << std::endl;
    }
}

Scene SceneBuilder::getScene()
{
    return scene;
}
