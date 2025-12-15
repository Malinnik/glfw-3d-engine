#include "crosshair.h"
#include <glad/glad.h>
#include "graphics/shader.h"

Crosshair::Crosshair()
{
    float vertices[] = {
        -0.01f, -0.01f,
         0.01f,  0.01f,

        -0.01f,  0.01f,
         0.01f, -0.01f,
    };

    int attrs[] = {2,0};

    shader = new Shader("./assets/shaders/crosshair.vert", "./assets/shaders/crosshair.frag");
    mesh = new Mesh(vertices, 4, attrs);
}

Crosshair::~Crosshair()
{
    delete shader;
    delete mesh;
    
}

void Crosshair::draw()
{
    glLineWidth(2.0f);
    shader->use();
    mesh->draw(GL_LINES);
}
