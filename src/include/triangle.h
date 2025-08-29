#pragma  once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "transform.h"


class Triangle {
public:
    // Triangle();
    Triangle(const std::string& vertexPath, const std::string& fragmentPath);
    ~Triangle();
    void draw();

    Transform transform;
    Shader shader;

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};
