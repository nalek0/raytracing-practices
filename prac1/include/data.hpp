#pragma once

#include <fstream>
#include <string>
#include <vector>

typedef struct color color_t;
typedef struct ray ray_t;
typedef struct Quaternion quternion_t;
typedef struct intersection_result intersection_result_t;

struct color
{
    float red;
    float green;
    float blue;
};

class Point
{
private:
public:
    float x;
    float y;
    float z;
    Point() : x(0), y(0), z(0) {}
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float length() const;
    Point normalized() const;

    Point operator+(const Point &right) const;
    Point operator-(const Point &right) const;
    Point operator-() const;
    Point operator*(const float k) const;
    Point operator/(const float k) const;
    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

class Quaternion
{
public:
    Point p;
    float w;

    Quaternion() : p(Point()), w(1) {}
    Quaternion(Point _p, float _w) : p(_p), w(_w) {}

    Quaternion conjugate() const;
    Quaternion operator*(const Quaternion &other);
};

struct ray
{
    Point position;
    Point direction;
};

struct intersection_result
{
    bool success;
    Point result;
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
    Point center_position;
    quternion_t rotation;
    color_t color;

    virtual struct intersection_result intersect(const ray_t ray) = 0;
};

class Plane : public Primitive
{
private:
    Point normal_direction;

public:
    Plane(Point _normal_direction);
    struct intersection_result intersect(const ray_t ray);
};

class Ellipsoid : public Primitive
{
private:
    float rx;
    float ry;
    float rz;

public:
    Ellipsoid(float _rx, float _ry, float _rz);
    struct intersection_result intersect(const ray_t ray);
};

class Box : public Primitive
{
private:
    float sizex;
    float sizey;
    float sizez;

public:
    Box(float _sizex, float _sizey, float _sizez);
    struct intersection_result intersect(const ray_t ray);
};

class Scene
{
public:
    int WIDTH;
    int HEIGHT;
    color_t BACKGROUND_COLOR;
    Point CAMERA_POSITION;
    Point CAMERA_RIGHT;
    Point CAMERA_UP;
    Point CAMERA_FORWARD;
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

float scalarMultiplication(const Point &left, const Point &right);
Point vectorMultiplication(const Point &left, const Point &right);
Point componentDivision(const Point &left, const Point &right);
Point rotate(const Point &p, const Quaternion &q);
