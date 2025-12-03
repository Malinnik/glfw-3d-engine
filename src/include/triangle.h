#pragma  once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "transform.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>


class Triangle {
public:
    Triangle();
    Triangle(Shader* shader);
    ~Triangle();
    void draw(Camera* camera);

    Transform transform;
    Shader *shader;

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};
