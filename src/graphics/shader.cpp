#include <iostream>
#include <fstream>
#include <sstream>

#include <loguru.hpp>
#include <glm/ext.hpp>
#include <fmt/format.h>

#include "files/file.h"
#include "shader.h"
#include "graphics/renderer/opengl/renderer.h"

// Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
//     compileShader(vertexShaderSource, fragmentShaderSource);
// }

Shader::Shader() {
    const GLchar *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 position;\n"
      "void main()\n"
      "{\n"
          "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
      "}\0";
    const GLchar *fragmentShaderSource = 
      "#version 330 core\n"
      "out vec4 color;\n"
      "void main()\n"
      "{\n"
          "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";

    compileShader(vertexShaderSource, fragmentShaderSource);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    LOG_F(INFO, fmt::format("VERTEX SHADER:: {}", vertexPath, fragmentPath).c_str());
    LOG_F(INFO, fmt::format("FRAGMENT SHADER:: {}\n", vertexPath, fragmentPath).c_str());
    std::string vertexCode = FileReader::readText(vertexPath);
    std::string fragmentCode = FileReader::readText(fragmentPath);
    compileShader(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader() {
    renderer::opengl::GLShader::del(ID);
}

void Shader::use() {
    renderer::opengl::GLShader::use(ID);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix)
{
    renderer::opengl::GLShader::uniformMatrix(name.c_str(), matrix, ID);
}

void Shader::uniformInt(const std::string& name, int value)
{
    renderer::opengl::GLShader::uniformInt(name.c_str(), value, ID);
}

void Shader::compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    LOG_F(1, "--------------------------- COMPILING SHADERS -------------------------------");
    LOG_F(1, fmt::format("\n ------ VERTEX SHADER ------\n {} \n\n", vertexShaderSource).c_str());
    LOG_F(1, fmt::format("\n ------ FRAGMENT SHADER ------\n {} \n\n", fragmentShaderSource).c_str());
    
    // Загрузка и компиляция шейдеров
    ID = renderer::opengl::GLShader::compile(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}