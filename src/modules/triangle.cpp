#include "triangle.h"

// Triangle::Triangle(): shader(), transform()
// {
//     setupMesh();
// }

Triangle::Triangle(const std::string& vertexPath, const std::string& fragmentPath): transform(), shader(vertexPath, fragmentPath)
{
    this->shader = shader;
    setupMesh();
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Triangle::setupMesh() {
    GLfloat vertices[] = {
        0.5f,  0.5f,  0.0f, // Верхний правый угол
        0.5f,  -0.5f, 0.0f, // Нижний правый угол
        -0.5f, -0.5f, 0.0f, // Нижний левый угол
        -0.5f, 0.5f,  0.0f  // Верхний левый угол
    };

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Triangle::draw() {
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}