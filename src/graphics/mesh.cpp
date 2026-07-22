#include <glad/glad.h>

#include "mesh.h"
#include "graphics/renderer/opengl/renderer.h"


Mesh::Mesh(const float *buffer, size_t vertices, const int *attrs) : vertices(vertices)
{
    vertexSize = 0;

    for (int i = 0; attrs[i]; i++)
    {
        vertexSize += attrs[i];
    }
    renderer::opengl::Mesh::create(buffer, vertices, attrs, vao, vbo, vertexSize);
}

Mesh::~Mesh()
{
    renderer::opengl::Mesh::del(vao, vbo);
}

void Mesh::reload(const float *buffer, size_t vertices)
{
    renderer::opengl::Mesh::reload(buffer, vertices, vao, vbo, vertexSize);
    this->vertices = vertices;
}

void Mesh::draw(unsigned int primitive)
{
    renderer::opengl::Mesh::draw(primitive, vao, vertices);
}