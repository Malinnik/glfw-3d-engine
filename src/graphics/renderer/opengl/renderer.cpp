#include "renderer.h"

void renderer::opengl::Mesh::create(const float *buffer, size_t vertices, const int *attrs, unsigned int &vao, unsigned int &vbo, size_t vertexSize)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexSize * vertices, buffer, GL_STATIC_DRAW);

    int offset = 0;
    for (int i = 0; attrs[i]; i++)
    {
        int size = attrs[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, sizeof(float)*vertexSize, (GLvoid*)(offset*sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);

}

void renderer::opengl::Mesh::draw(unsigned int primitive, unsigned int vao, size_t vertices)
{
    glBindVertexArray(vao);
    glDrawArrays(primitive, 0, vertices);
    glBindVertexArray(0);
}

void renderer::opengl::Mesh::reload(const float *buffer, size_t vertices, unsigned int vao, unsigned int vbo, size_t vertexSize)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);
}

void renderer::opengl::Mesh::del(unsigned int &vao, unsigned int &vbo)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

GLuint renderer::opengl::GLShader::compile(const char *vShaderCode, const char *fShaderCode)
{
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
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Удаляем шейдеры, так как они уже связаны с программой и больше не нужны
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return ID;
}

void renderer::opengl::GLShader::checkCompileErrors(GLuint shader, const char *type)
{
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

void renderer::opengl::GLShader::use(GLuint ID)
{
    glUseProgram(ID);
}

void renderer::opengl::GLShader::del(GLuint ID)
{
    glDeleteProgram(ID);
}

void renderer::opengl::GLShader::uniformMatrix(const char *name, glm::mat4 matrix, GLuint ID)
{
    GLuint transformLoc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void renderer::opengl::GLShader::uniformInt(const char *name, int value, GLuint ID)
{
    GLint loc = glGetUniformLocation(ID, name);
    if (loc >= 0)
        glUniform1i(loc, value);
}
