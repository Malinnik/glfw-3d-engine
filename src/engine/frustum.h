#pragma once

#include "glm/glm.hpp"
#include <array>


struct Plane {
    glm::vec3 normal = {0.0f, 0.0f, 0.0f};
    float distance = 0.0f;

    float signedDistance(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};

class Frustum {
    public:
        std::array<Plane, 6> planes;

        void extractFromMatrix(const glm::mat4& projview);
        bool isAABBVisible(const glm::vec3& min, const glm::vec3& max) const;
};
