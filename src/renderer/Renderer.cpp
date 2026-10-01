#include "renderer/Renderer.h"

#include <iostream>


// ============================================================
// Vertex Shader
// ============================================================

static const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    gl_PointSize = 12.0;
}
)";


// ============================================================
// Fragment Shader
// ============================================================

static const char* fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";


// ============================================================
// Constructor
// ============================================================

Renderer::Renderer()
    : shaderProgram(0),
      vao(0),
      vbo(0),
      projectionLocation(-1),
      viewLocation(-1),
      modelLocation(-1)
{
}


// ============================================================
// Destructor
// ============================================================

Renderer::~Renderer()
{
    if (vbo != 0)
        glDeleteBuffers(1, &vbo);

    if (vao != 0)
        glDeleteVertexArrays(1, &vao);

    if (shaderProgram != 0)
        glDeleteProgram(shaderProgram);
}


// ============================================================
// Initialize
// ============================================================

bool Renderer::initialize()
{
    // --------------------------------------------------------
    // Create shaders
    // --------------------------------------------------------

    if (!createShaders())
    {
        return false;
    }


    // --------------------------------------------------------
    // Create VAO
    // --------------------------------------------------------

    glGenVertexArrays(
        1,
        &vao
    );

    glBindVertexArray(vao);


    // --------------------------------------------------------
    // Create VBO
    // --------------------------------------------------------

    glGenBuffers(
        1,
        &vbo
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        vbo
    );


    // We will update this every frame
    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );


    // --------------------------------------------------------
    // Position attribute
    // --------------------------------------------------------

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 3,
        nullptr
    );

    glEnableVertexAttribArray(0);


    // --------------------------------------------------------
    // Unbind
    // --------------------------------------------------------

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    glBindVertexArray(0);


    // --------------------------------------------------------
    // Uniform locations
    // --------------------------------------------------------

    projectionLocation =
        glGetUniformLocation(
            shaderProgram,
            "uProjection"
        );

    viewLocation =
        glGetUniformLocation(
            shaderProgram,
            "uView"
        );

    modelLocation =
        glGetUniformLocation(
            shaderProgram,
            "uModel"
        );


    return true;
}


// ============================================================
// Create shaders
// ============================================================

bool Renderer::createShaders()
{
    shaderProgram =
        createShaderProgram(
            vertexShaderSource,
            fragmentShaderSource
        );

    return shaderProgram != 0;
}


// ============================================================
// Compile shader
// ============================================================

GLuint Renderer::compileShader(
    GLenum type,
    const char* source
)
{
    GLuint shader =
        glCreateShader(type);


    glShaderSource(
        shader,
        1,
        &source,
        nullptr
    );


    glCompileShader(shader);


    // --------------------------------------------------------
    // Check compilation
    // --------------------------------------------------------

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

        std::cerr
            << "Shader compilation failed:\n"
            << infoLog
            << '\n';


        glDeleteShader(shader);

        return 0;
    }


    return shader;
}


// ============================================================
// Create shader program
// ============================================================

GLuint Renderer::createShaderProgram(
    const char* vertexSource,
    const char* fragmentSource
)
{
    GLuint vertexShader =
        compileShader(
            GL_VERTEX_SHADER,
            vertexSource
        );


    if (vertexShader == 0)
        return 0;


    GLuint fragmentShader =
        compileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource
        );


    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);

        return 0;
    }


    GLuint program =
        glCreateProgram();


    glAttachShader(
        program,
        vertexShader
    );

    glAttachShader(
        program,
        fragmentShader
    );


    glLinkProgram(program);


    // --------------------------------------------------------
    // Check linking
    // --------------------------------------------------------

    GLint success;

    glGetProgramiv(
        program,
        GL_LINK_STATUS,
        &success
    );


    if (!success)
    {
        char infoLog[1024];

        glGetProgramInfoLog(
            program,
            sizeof(infoLog),
            nullptr,
            infoLog
        );

        std::cerr
            << "Shader linking failed:\n"
            << infoLog
            << '\n';


        glDeleteProgram(program);

        program = 0;
    }


    glDeleteShader(vertexShader);

    glDeleteShader(fragmentShader);


    return program;
}

//Render Frame 
void Renderer::renderFrame(
    const AnimationFrame& frame,
    const Skeleton& skeleton
)
{
    (void)skeleton;

    glUseProgram(shaderProgram);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        frame.nodes.size() * sizeof(NodePosition),
        frame.nodes.data(),
        GL_DYNAMIC_DRAW
    );

    GLint colorLocation =
        glGetUniformLocation(
            shaderProgram,
            "uColor"
        );

    glUniform3f(
        colorLocation,
        1.0f,
        1.0f,
        1.0f
    );

    glDrawArrays(
        GL_POINTS,
        0,
        static_cast<GLsizei>(frame.nodes.size())
    );

    glBindVertexArray(0);
}