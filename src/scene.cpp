#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

#include "scene.hpp"

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
    else if (command.getCommandName() == "IOR")
    {
        assert(is_primitive_building);

        current_primitive->IOR = std::stof(command.getArgs().at(0));
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

Ray generate_ray(const Scene &scene, const int x, const int y)
{
    float px = (2. * (0.5 + x) / scene.WIDTH - 1) * tan(scene.FOV_X / 2);
    float py = -1 * (2. * (0.5 + y) / scene.HEIGHT - 1) * tan(scene.FOV_Y / 2);
    Point pixel_direction = scene.CAMERA_FORWARD.normalized() + scene.CAMERA_RIGHT.normalized() * px + scene.CAMERA_UP.normalized() * py;
    Ray pixel_ray = {scene.CAMERA_POSITION, pixel_direction};

    return pixel_ray;
}

RayCollision first_intersection(const Scene &scene, const Ray &ray, float coeff_limit)
{
    float ray_coeff = coeff_limit;
    Primitive *result_primitive = nullptr;
    IntersectionResult result_intersection = {.success = false};

    for (Primitive *primitive : scene.primitives)
    {
        IntersectionResult result = primitive->intersect(ray, ray_coeff);

        if (result.success)
        {
            result_primitive = primitive;
            result_intersection = result;
            ray_coeff = result.direction_coeff;
        }
    }

    return {
        .primitive = result_primitive,
        .intersection = result_intersection};
}

Intensity apply_attenuation(const PointLight &light, const Point &point)
{
    Color I = light.intensity;
    Point D = light.position - point;
    float R = D.length();
    float U = light.attenuation.red + light.attenuation.green * R + light.attenuation.blue * R * R;

    return I / U;
}

Color diffuser_color(const Scene &scene, const Ray &ray, const RayCollision &collision)
{
    Color result_intensivity = {0, 0, 0};
    result_intensivity += scene.AMBIENT_LIGHT;

    for (DirectionLight light : scene.directioned_lights)
    {
        Point intersection = collision.intersection.point;
        Point direction = light.direction;
        Ray ray = {
            .position = intersection + direction * 1e-4,
            .direction = direction};

        RayCollision shadow_collision = first_intersection(scene, ray, 1000);

        if (shadow_collision.intersection.success)
        {
            continue; // In the shadow
        }
        else
        {
            Point N = light.direction.normalized();
            Point L = collision.intersection.normale.normalized();
            float cos_angle = scalarMultiplication(N, L);

            if (cos_angle > 0)
                result_intensivity += light.intensity * cos_angle;
        }
    }

    for (PointLight light : scene.pointed_lights)
    {
        Point intersection = collision.intersection.point;
        Point direction = light.position - intersection;
        Ray ray = {
            .position = intersection + direction * 1e-4,
            .direction = direction};

        RayCollision shadow_collision = first_intersection(scene, ray, 1);

        if (shadow_collision.intersection.success)
            continue; // In the shadow
        else
        {
            Point L = direction.normalized();
            Point N = collision.intersection.normale.normalized();
            Intensity apllied_attenuation = apply_attenuation(light, intersection);
            float cos_angle = scalarMultiplication(L, N);

            if (cos_angle > 0)
                result_intensivity += apllied_attenuation * cos_angle;
        }
    }

    float red = collision.primitive->color.red * result_intensivity.red;
    float green = collision.primitive->color.green * result_intensivity.green;
    float blue = collision.primitive->color.blue * result_intensivity.blue;

    return {
        .red = red,
        .green = green,
        .blue = blue};
}

Color metallic_color(const Scene &scene, const Ray &ray, const RayCollision &collision, int depth)
{
    if (depth > scene.RAY_DEPTH)
        return {0, 0, 0};

    Point point = collision.intersection.point;
    Point direction = ray.direction;
    Point normale = collision.intersection.normale;
    Point reflection = get_reflection(direction, normale);
    Ray new_ray = {
        .position = point + reflection * 1e-4,
        .direction = reflection};

    Intensity reflection_color = ray_color(scene, new_ray, 1000, depth + 1);

    return collision.primitive->color * reflection_color;
}

Color dielectric_color(const Scene &scene, const Ray &ray, const RayCollision &collision, int depth)
{
    if (depth > scene.RAY_DEPTH)
        return {0, 0, 0};

    Point point;
    Point direction;
    Point normale;
    float ior1;
    float ior2;

    if (collision.intersection.inside_primitive)
    {
        point = collision.intersection.point;
        direction = ray.direction;
        normale = -collision.intersection.normale;
        ior1 = collision.primitive->IOR;
        ior2 = 1;
    }
    else
    {
        point = collision.intersection.point;
        direction = ray.direction;
        normale = collision.intersection.normale;
        ior1 = 1;
        ior2 = collision.primitive->IOR;
    }

    Point D = direction.normalized();
    Point N = normale.normalized();
    Point L = -D;
    float ior_div = ior1 / ior2;
    float cos1 = scalarMultiplication(L, N);
    float sin2 = ior_div * sqrt(1 - cos1 * cos1);

    if (sin2 > 1)
        return metallic_color(scene, ray, collision, depth);

    float cos2 = sqrt(1 - sin2 * sin2);
    Point refraction = D * ior_div + N * (ior_div * cos1 - cos2);
    Point reflection = get_reflection(direction, normale);
    Ray refraction_ray = {
        .position = point + refraction * 1e-4,
        .direction = refraction};
    Ray reflection_ray = {
        .position = point + reflection * 1e-4,
        .direction = reflection};

    float R0 = ((ior1 - ior2) / (ior1 + ior2));
    R0 *= R0;
    float powered = 1 - scalarMultiplication(L, N);
    powered = powered * powered * powered * powered * powered;

    float reflection_light = R0 + (1 - R0) * powered;
    float refraction_light = 1 - reflection_light;

    Color reflection_color = ray_color(scene, reflection_ray, 1000, depth + 1);
    Color refraction_color = ray_color(scene, refraction_ray, 1000, depth + 1);
    Color result = reflection_color * reflection_light + refraction_color * refraction_light;

    if (collision.intersection.inside_primitive)
        return reflection_color * reflection_light + refraction_color * refraction_light;
    else
        return reflection_color * reflection_light + refraction_color * refraction_light * collision.primitive->color;
}

Color ray_color(const Scene &scene, const Ray &ray, float coeff_limit, int depth)
{
    RayCollision collision = first_intersection(scene, ray, coeff_limit);

    if (collision.intersection.success)
    {
        switch (collision.primitive->material)
        {
        case Material::METALLIC:
            return metallic_color(scene, ray, collision, depth);
        case Material::DIELECTRIC:
            return dielectric_color(scene, ray, collision, depth);
        case Material::DIFFUSER:
            return diffuser_color(scene, ray, collision);
        default:
            std::cerr << "Unexpected material type " << collision.primitive->material;

            return collision.primitive->color;
        }
    }
    else
        return scene.BACKGROUND_COLOR;
}
