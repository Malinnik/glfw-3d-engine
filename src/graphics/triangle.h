#pragma  once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "engine/transform.h"
#include "graphics/shader.h"
#include "engine/camera.h"


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
