#pragma once
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <loguru.hpp>
#include <fmt/format.h>


namespace renderer {
    namespace opengl {
        class Mesh {
            public:
                static void create(const float *buffer, size_t vertices, const int *attrs, unsigned int &vao, unsigned int &vbo, size_t vertexSize);
                static void draw(unsigned int primitive, unsigned int vao, size_t vertices);
                static void reload(const float *buffer, size_t vertices, unsigned int vao, unsigned int vbo, size_t vertexSize);
                static void del(unsigned int &vao, unsigned int &vbo);
        };

        class GLShader {
            public:
                static GLuint compile(const char* vShaderCode, const char* fShaderCode);
                static void checkCompileErrors(GLuint shader, const char* type);
                static void use(GLuint ID);
                static void del(GLuint ID);

                static void uniformMatrix(const char* name, glm::mat4 matrix, GLuint ID);
                static void uniformInt(const char* name, int value, GLuint ID);
        };
    }
}