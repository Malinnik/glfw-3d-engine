#pragma once
#include "glm/vec3.hpp"

class Transform {
    public:
        Transform(float x, float y, float z);
        Transform();
        glm::vec3 position;

        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3* position);
        glm::vec3* getPosition();
};