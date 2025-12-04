#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    // Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    Shader();
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use();
    void uniformMatrix(std::string name, glm::mat4 matrix);

private:
    GLuint ID;
    void compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    void checkCompileErrors(GLuint shader, std::string type);
};
