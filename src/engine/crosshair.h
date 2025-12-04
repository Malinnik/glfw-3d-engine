#pragma once

#include "graphics/shader.h"
#include "graphics/mesh.h"


class Crosshair{
    Shader* shader;
    Mesh* mesh;

    public:
        Crosshair();
        ~Crosshair();
        void draw();
};