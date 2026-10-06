#pragma once
#include <stdio.h>
#include "string"
#include "glad/gl.h"
#include "renderer/Shader.h"

class BackgroundRenderer
{
public:
    BackgroundRenderer();
    ~BackgroundRenderer();

    bool initialize();

    void render(const std::string& background_path);

private:
    GLuint vao;
    GLuint vbo;
    Shader backgroundShader;

    GLuint texture;
};