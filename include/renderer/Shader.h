#pragma once

#include <string>
#include <unordered_map>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
    Shader(const std::string& shaderName);

    void use() const;

    void setVec3(const std::string& name, float x, float y, float z) const;

    void setMat4(const std::string& name, glm::mat4& matrix) const;

    GLuint getProgram() const { return programID; }

private:
    void createShader(const std::string& shaderName);

    GLuint compileShader(GLenum type, const std::string& source);

    std::string readFile(const std::string& path);

private:
    GLuint programID = 0;
};