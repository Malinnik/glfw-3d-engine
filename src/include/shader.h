#pragma once
#include <glad/glad.h>
#include <string>


class Shader {
public:
    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    Shader();
    ~Shader();

    void use();

private:
    GLuint ID;
    void compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    void checkCompileErrors(GLuint shader, std::string type);
};
