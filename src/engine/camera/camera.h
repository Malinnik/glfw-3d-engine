#pragma once
#include <glm/glm.hpp>
#include "engine/frustum.h"

using namespace glm;


class Camera {
    void updateVectors();

    public:
        vec3 front;
        vec3 up;
        vec3 right;

        vec3 position;
        float fov;
        mat4 rotation;

        float camX = 0.0f;
        float camY = 0.0f;

        float speed = 10.0f;
        
        Frustum frustum;

        Camera(vec3 position, float fov);
        ~Camera();

        void rotate(float x, float y, float z);
        mat4 getProjection();
        mat4 getView();
        void updateFrustum();
};