#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// --------------------------------------------------
// Vertex Shader
// --------------------------------------------------

const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";


// --------------------------------------------------
// Fragment Shader
// --------------------------------------------------

const char* fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D backgroundTexture;

void main()
{
    FragColor = texture(backgroundTexture, TexCoord);
}
)";


// --------------------------------------------------
// Window resize callback
// --------------------------------------------------

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
)
{
    glViewport(0, 0, width, height);
}


// --------------------------------------------------
// Compile shader
// --------------------------------------------------

GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(
        shader,
        1,
        &source,
        nullptr
    );

    glCompileShader(shader);

    int success;
    char infoLog[512];

    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        glGetShaderInfoLog(
            shader,
            512,
            nullptr,
            infoLog
        );

        std::cerr
            << "Shader compilation failed:\n"
            << infoLog
            << std::endl;
    }

    return shader;
}


// --------------------------------------------------
// Create shader program
// --------------------------------------------------

GLuint createShaderProgram()
{
    GLuint vertexShader =
        compileShader(
            GL_VERTEX_SHADER,
            vertexShaderSource
        );

    GLuint fragmentShader =
        compileShader(
            GL_FRAGMENT_SHADER,
            fragmentShaderSource
        );

    GLuint shaderProgram =
        glCreateProgram();

    glAttachShader(
        shaderProgram,
        vertexShader
    );

    glAttachShader(
        shaderProgram,
        fragmentShader
    );

    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (!success)
    {
        glGetProgramInfoLog(
            shaderProgram,
            512,
            nullptr,
            infoLog
        );

        std::cerr
            << "Shader linking failed:\n"
            << infoLog
            << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main()
{
    // ----------------------------------------------
    // Initialize GLFW
    // ----------------------------------------------

    if (!glfwInit())
    {
        std::cerr
            << "Failed to initialize GLFW"
            << std::endl;

        return -1;
    }


    // ----------------------------------------------
    // Tell GLFW which OpenGL version we want
    // ----------------------------------------------

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );


    // ----------------------------------------------
    // Create window
    // ----------------------------------------------

    GLFWwindow* window =
        glfwCreateWindow(
            1280,
            720,
            "Shadow Fight",
            nullptr,
            nullptr
        );

    if (!window)
    {
        std::cerr
            << "Failed to create GLFW window"
            << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);


    // ----------------------------------------------
    // Setup resize callback
    // ----------------------------------------------

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );


    // ----------------------------------------------
    // Initialize GLAD
    // ----------------------------------------------

    if (!gladLoadGLLoader(
            (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr
            << "Failed to initialize GLAD"
            << std::endl;

        glfwTerminate();

        return -1;
    }


    // ----------------------------------------------
    // Create shader program
    // ----------------------------------------------

    GLuint shaderProgram =
        createShaderProgram();


    // ----------------------------------------------
    // Fullscreen rectangle
    //
    //        (-1,1)             (1,1)
    //           +---------------+
    //           |               |
    //           |               |
    //           |               |
    //           +---------------+
    //        (-1,-1)           (1,-1)
    //
    // ----------------------------------------------

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


    // ----------------------------------------------
    // Create VAO and VBO
    // ----------------------------------------------

    GLuint VAO;
    GLuint VBO;

    glGenVertexArrays(
        1,
        &VAO
    );

    glGenBuffers(
        1,
        &VBO
    );


    // Bind VAO

    glBindVertexArray(VAO);


    // Bind VBO

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    // Upload vertices

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // ----------------------------------------------
    // Position attribute
    // ----------------------------------------------

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ----------------------------------------------
    // Texture coordinate attribute
    // ----------------------------------------------

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);


    // ----------------------------------------------
    // Load background image
    // ----------------------------------------------

    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image =
        stbi_load(
            "C:/Users/Akshay/Desktop/ShadowFight/assets/background.png",
            &width,
            &height,
            &channels,
            4
        );

    if (!image)
    {
        std::cerr
            << "Failed to load background.png"
            << std::endl;

        glfwTerminate();

        return -1;
    }

    std::cout
        << "Loaded background: "
        << width
        << " x "
        << height
        << std::endl;


    // ----------------------------------------------
    // Create OpenGL texture
    // ----------------------------------------------

    GLuint texture;

    glGenTextures(
        1,
        &texture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        texture
    );


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


    // Upload image to GPU

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


    // Generate mipmaps

    glGenerateMipmap(
        GL_TEXTURE_2D
    );


    // We no longer need CPU image data

    stbi_image_free(image);


    // ----------------------------------------------
    // Tell shader which texture unit to use
    // ----------------------------------------------

    glUseProgram(shaderProgram);

    glUniform1i(
        glGetUniformLocation(
            shaderProgram,
            "backgroundTexture"
        ),
        0
    );


    // ----------------------------------------------
    // Main game loop
    // ----------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // Process keyboard/window events

        glfwPollEvents();


        // ------------------------------------------
        // Clear screen
        // ------------------------------------------

        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT
        );


        // ------------------------------------------
        // Activate texture unit 0
        // ------------------------------------------

        glActiveTexture(
            GL_TEXTURE0
        );


        // Bind background

        glBindTexture(
            GL_TEXTURE_2D,
            texture
        );


        // ------------------------------------------
        // Draw background
        // ------------------------------------------

        glUseProgram(
            shaderProgram
        );

        glBindVertexArray(
            VAO
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            6
        );


        // ------------------------------------------
        // Show frame
        // ------------------------------------------

        glfwSwapBuffers(window);
    }


    // ----------------------------------------------
    // Cleanup
    // ----------------------------------------------

    glDeleteTextures(
        1,
        &texture
    );

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteProgram(
        shaderProgram
    );

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}