#include "transform.h"

Transform::Transform() : position(){}

Transform::Transform(float x, float y, float z) : position(x, y ,z)
{}

void Transform::setPosition(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}

void Transform::setPosition(glm::vec3* position)
{
    this->position = *position;
}

glm::vec3* Transform::getPosition()
{
    return &position;
}