#include "triangle.h"

// Triangle::Triangle(): shader(), transform()
// {
//     setupMesh();
// }

Triangle::Triangle(): transform(), shader()
{
    setupMesh();
}

Triangle::Triangle(Shader *shader): transform(), shader(shader)
{
    setupMesh();
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Triangle::setupMesh() {
    GLfloat vertices[] = {
        // x    y     z     u     v
       -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,  // 0
        1.0f,-1.0f, 0.0f, 1.0f, 0.0f,  // 1
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // 2
       -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 3
    };

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};  // 2 треугольника

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glClearColor(0.6f, 0.62f, 0.65f, 1);

    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Triangle::draw(Camera* camera) {
    glm::mat4 model(1.0f);
    // model = glm::translate(model, glm::vec3(0.5f, 0, 0));
    shader->uniformMatrix("model", model);
    shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
    shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}