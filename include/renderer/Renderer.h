#pragma once

#include <glad/gl.h>
#include <cstdint>

#include "animation/Animation.h"
#include "renderer/Skeletion.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize();

    void renderFrame(
        const AnimationFrame& frame,
        const Skeleton& skeleton
    );

private:
    bool createShaders();

    GLuint compileShader(
        GLenum type,
        const char* source
    );

    GLuint createShaderProgram(
        const char* vertexSource,
        const char* fragmentSource
    );

private:
    GLuint shaderProgram;

    GLuint vao;
    GLuint vbo;

    GLint projectionLocation;
    GLint viewLocation;
    GLint modelLocation;
};