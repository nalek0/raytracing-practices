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

    struct point operator+(const struct point &right)
    {
        return {
            x + right.x,
            y + right.y,
            z + right.z};
    }

    struct point operator-(const struct point &right)
    {
        return {
            x - right.x,
            y - right.y,
            z - right.z};
    }

    struct point operator-()
    {
        return {-x, -y, -z};
    }

    struct point operator*(const float k)
    {
        return {k * x, k * y, k * z};
    }

    // scalar multiplication
    float operator*(struct point & right)
    {
        return x * right.x + y * right.y + z * right.z;
    }

    friend std::ostream &operator<<(std::ostream &os, const struct point &p)
    {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
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
    struct point center_position;
    quternion_t rotation;
    color_t color;

    virtual struct point intersect(const ray_t ray) = 0;
};

class Plane : public Primitive
{
private:
    struct point normal_direction;

public:
    Plane(struct point _normal_direction);
    struct point intersect(const ray_t ray);
};

class Ellipsoid : public Primitive
{
private:
    float rx;
    float ry;
    float rz;

public:
    Ellipsoid(float _rx, float _ry, float _rz);
    struct point intersect(const ray_t ray);
};

class Box : public Primitive
{
private:
    float sizex;
    float sizey;
    float sizez;

public:
    Box(float _sizex, float _sizey, float _sizez);
    struct point intersect(const ray_t ray);
};

class Scene
{
public:
    float WIDTH;
    float HEIGHT;
    color_t BACKGROUND_COLOR;
    struct point CAMERA_POSITION;
    struct point CAMERA_RIGHT;
    struct point CAMERA_UP;
    struct point CAMERA_FORWARD;
    float FOV_X;
    float FOV_Y;
    std::vector<Primitive *> primitives;

    Scene();
    void checkData();
};

class SceneBuilder
{
private:
    Scene scene;
    Primitive *current_primitive;
    bool is_primitive_building;

public:
    SceneBuilder();
    void acceptCommand(const Command &command);
    Scene getScene();
};
