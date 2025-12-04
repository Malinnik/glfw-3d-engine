#pragma once
#include "glm/vec3.hpp"

class Transform {
    private:
        glm::vec3 _position;

    public:
        Transform(float x, float y, float z);
        Transform();

        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3* position);
        glm::vec3* getPosition();
};