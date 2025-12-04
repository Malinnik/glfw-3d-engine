#pragma  once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "engine/transform.h"
#include "engine/camera.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"



class Triangle {
public:
    Triangle();
    Triangle(Shader* shader);
    ~Triangle();
    void draw(Camera* camera);

    Transform transform;
    Shader *shader;

private:
    Mesh* mesh;
    void setupMesh();
};
