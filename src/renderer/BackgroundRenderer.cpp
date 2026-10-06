#include "renderer/BackgroundRenderer.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


BackgroundRenderer::BackgroundRenderer()
    : backgroundShader("background")
{
}

BackgroundRenderer::~BackgroundRenderer()
{
    if (vbo != 0)
        glDeleteBuffers(1, &vbo);

    if (vao != 0)
        glDeleteVertexArrays(1, &vao);
}


bool BackgroundRenderer::initialize()
{
    // --------------------------------------------------
    // Fullscreen quad
    // --------------------------------------------------

    float vertices[] =
    {
        // Position              // Texture coordinates

        -1.0f,  1.0f, 0.0f,       0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,       0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,       1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,       0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,       1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


void BackgroundRenderer::render(const std::string& background_path)
{
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load(
        background_path.c_str(),
        &width,
        &height,
        &channels,
        4
    );

    if (!image)
    {
        std::cerr
            << "Failed to load background: "
            << background_path
            << std::endl;
        return;
    }


    // Create texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    // Texture wrapping
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );


    // Texture filtering
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );


    // Upload image
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image
    );


    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
    backgroundShader.use();


    // --------------------------------------------------
    // Tell shader to use texture unit 0
    // --------------------------------------------------

    GLint textureLocation =
        glGetUniformLocation(
            backgroundShader.getProgram(),
            "backgroundTexture"
        );

    glUniform1i(textureLocation, 0);


    // --------------------------------------------------
    // Bind texture
    // --------------------------------------------------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    // --------------------------------------------------
    // Draw fullscreen quad
    // --------------------------------------------------
    glBindVertexArray(vao);
    glDrawArrays(
        GL_TRIANGLES,
        0,
        6
    );

    glBindVertexArray(0);
    // --------------------------------------------------
    // Cleanup texture
    // --------------------------------------------------
    glDeleteTextures(1, &texture);
}