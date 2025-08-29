#include "shader.h"
#include "file.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

void Shader::compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    std::cout << "\n\n\n" << "--------------------------- COMPILING SHADERS -------------------------------" << "\n\n\n";
    std::cout << "\n ------ VERTEX SHADER ------\n" << vertexShaderSource << "\n\n";
    std::cout << "\n ------ FRAGMENT SHADER ------\n"<< fragmentShaderSource << "\n\n";
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
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}