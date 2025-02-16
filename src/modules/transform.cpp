#include "transform.h"
#include "vec.h"

Transform::Transform() : _position(){}

Transform::Transform(float x, float y, float z) 
    : _position(x, y ,z)
    {}

void Transform::setPosition(float x, float y, float z)
{
    _position = Vec3(x, y, z);
}

void Transform::setPosition(Vec3* position)
{
    _position = *position;
}

Vec3* Transform::getPosition()
{
    return &_position;
}