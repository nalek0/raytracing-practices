#pragma once

#include <fstream>
#include <string>
#include <vector>

typedef struct color color_t;
typedef struct point point_t;
typedef struct ray ray_t;
typedef struct quternion quternion_t;

struct color
{
    float red;
    float green;
    float blue;
};

struct point
{
    float x;
    float y;
    float z;
};

struct ray
{
    struct point position;
    struct point direction;
};

struct quternion
{
    float x;
    float y;
    float z;
    float w;
};

class Command
{
private:
    std::string command_name;
    std::vector<float> args;

public:
    Command(std::string _name, std::vector<float> _args);
    std::string getCommandName() const;
    std::vector<float> getArgs() const;
};

class CommandScanner
{
private:
    std::string filename;

public:
    CommandScanner(std::string _filename);
    void parse(std::vector<Command> &result);
};

class Primitive
{
public:
    point_t center_position;
    quternion_t rotation;
    color_t color;

    virtual point_t intersect(const ray_t ray) = 0;
};

class Plane : public Primitive
{
private:
    point_t normal_direction;
public:
    Plane(point_t _normal_direction);
    point_t intersect(const ray_t ray);
};

class Ellipsoid : public Primitive
{
private:
    float rx;
    float ry;
    float rz;
public:
    Ellipsoid(float _rx, float _ry, float _rz);
    point_t intersect(const ray_t ray);
};

class Box : public Primitive
{
private:
    float sizex;
    float sizey;
    float sizez;
public:
    Box(float _sizex, float _sizey, float _sizez);
    point_t intersect(const ray_t ray);
};

class Scene
{
public:
    float WIDTH;
    float HEIGHT;
    color_t BACKGROUND_COLOR;
    point_t CAMERA_POSITION;
    point_t CAMERA_RIGHT;
    point_t CAMERA_UP;
    point_t CAMERA_FORWARD;
    float FOV_X;
    float FOV_Y;
    std::vector<Primitive*> primitives;

    Scene();
    void checkData();
};

class SceneBuilder
{
private:
    Scene scene;
    Primitive * current_primitive;
    bool is_primitive_building;

public:
    SceneBuilder();
    void acceptCommand(const Command &command);
    Scene getScene();
};
