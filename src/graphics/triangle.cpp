#include "triangle.h"

Triangle::Triangle(): transform(), shader()
{
    setupMesh();
}

Triangle::Triangle(Shader *shader): transform(), shader(shader)
{
    setupMesh();
}

Triangle::~Triangle() {
    delete(mesh);
}

void Triangle::setupMesh() {
    GLfloat vertices[] = {
        // x    y     z     u     v
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  
        
    };
    int attrs[] = {3,2,0};
    mesh = new Mesh(vertices, 6, attrs);
}

void Triangle::draw(Camera* camera) {
    glm::mat4 model(1.0f);
    // model = glm::translate(model, glm::vec3(0.5f, 0, 0));
    shader->uniformMatrix("model", model);
    shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
    shader->use();

    mesh->draw(GL_TRIANGLES);
}