#include <iostream>
#include <fstream>
#include <sstream>

#include <loguru.hpp>
#include <glm/ext.hpp>
#include <fmt/format.h>

#include "files/file.h"
#include "shader.h"

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
    std::string vertexCode = FileReader::readText(vertexPath);
    std::string fragmentCode = FileReader::readText(fragmentPath);

    compileShader(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix)
{
    GLuint transformLoc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uniformInt(const std::string& name, int value)
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc >= 0)
        glUniform1i(loc, value);
}

void Shader::compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    LOG_F(INFO, "--------------------------- COMPILING SHADERS -------------------------------");
    LOG_F(INFO, fmt::format("\n ------ VERTEX SHADER ------\n {} \n\n", vertexShaderSource).c_str());
    LOG_F(INFO, fmt::format("\n ------ FRAGMENT SHADER ------\n {} \n\n", fragmentShaderSource).c_str());
    // Загрузка и компиляция шейдеров
    const char* vShaderCode = vertexShaderSource.c_str();
    const char* fShaderCode = fragmentShaderSource.c_str();

    GLuint vertex, fragment;
    int success;
    char infoLog[512];

    // Вершинный шейдер
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Фрагментный шейдер
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Шейдерная программа
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Удаляем шейдеры, так как они уже связаны с программой и больше не нужны
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LOG_F(ERROR, fmt::format("ERROR::SHADER_COMPILATION_ERROR of type:  {} \n {} \n -- --------------------------------------------------- -- \n", type, infoLog).c_str());
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG_F(ERROR, fmt::format("ERROR::PROGRAM_LINKING_ERROR of type:  {} \n {} \n -- --------------------------------------------------- -- \n", type, infoLog).c_str());
        }
    }
}