#include "transform.h"

Transform::Transform() : _position(){}

Transform::Transform(float x, float y, float z) 
    : _position(x, y ,z)
    {}

void Transform::setPosition(float x, float y, float z)
{
    _position = glm::vec3(x, y, z);
}

void Transform::setPosition(glm::vec3* position)
{
    _position = *position;
}

glm::vec3* Transform::getPosition()
{
    return &_position;
}