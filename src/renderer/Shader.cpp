#include "renderer/Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>


Shader::Shader(const std::string& shaderName){
    createShader(shaderName);
}

std::string Shader::readFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open shader file: " + path
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Shader::compileShader(
    GLenum type,
    const std::string& source
)
{
    GLuint shader = glCreateShader(type);

    const char* sourceCStr = source.c_str();

    glShaderSource(
        shader,
        1,
        &sourceCStr,
        nullptr
    );

    glCompileShader(shader);

    GLint success;
    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        char infoLog[1024];

        glGetShaderInfoLog(
            shader,
            sizeof(infoLog),
            nullptr,
            infoLog
        );

        std::string shaderType =
            type == GL_VERTEX_SHADER
                ? "VERTEX"
                : "FRAGMENT";

        std::cerr
            << shaderType
            << " SHADER COMPILATION FAILED:\n"
            << infoLog
            << std::endl;

        glDeleteShader(shader);

        throw std::runtime_error(
            "Shader compilation failed"
        );
    }

    return shader;
}

void Shader::createShader(const std::string& shaderName)
{
    std::string vertexPath =
        "shaders/" + shaderName + ".vert";

    std::string fragmentPath =
        "shaders/" + shaderName + ".frag";

    // Read source
    std::string vertexSource =
        readFile(vertexPath);

    std::string fragmentSource =
        readFile(fragmentPath);

    // Compile
    GLuint vertexShader =
        compileShader(
            GL_VERTEX_SHADER,
            vertexSource
        );

    GLuint fragmentShader =
        compileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource
        );

    // Create program
    programID = glCreateProgram();

    glAttachShader(
        programID,
        vertexShader
    );

    glAttachShader(
        programID,
        fragmentShader
    );

    glLinkProgram(programID);

    GLint success;

    glGetProgramiv(
        programID,
        GL_LINK_STATUS,
        &success
    );

    if (!success)
    {
        char infoLog[1024];

        glGetProgramInfoLog(
            programID,
            sizeof(infoLog),
            nullptr,
            infoLog
        );

        std::cerr
            << "SHADER PROGRAM LINKING FAILED:\n"
            << infoLog
            << std::endl;

        glDeleteProgram(programID);
        programID = 0;
    }

    // Shaders are no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const
{
    glUseProgram(programID);
}

void Shader::setVec3(
    const std::string& name,
    float x,
    float y,
    float z
) const
{
    GLint location =
        glGetUniformLocation(
            programID,
            name.c_str()
        );

    glUniform3f(
        location,
        x,
        y,
        z
    );
}

void Shader::setMat4(const std::string& name, glm::mat4& matrix) const
{
    GLint location =
        glGetUniformLocation(
            programID,
            name.c_str()
        );

    glUniformMatrix4fv(
        location,
         1,
        GL_FALSE,
        glm::value_ptr(matrix)
    );
}