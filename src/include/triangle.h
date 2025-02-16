#pragma  once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "shader.h"


class Triangle {
public:
    Triangle(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Triangle();

    void draw();
    void setPosition(float x, float y, float z);

private:
    GLuint VAO, VBO, EBO;
    Shader shader;
    float position[3];

    void setupMesh();
};
