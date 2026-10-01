#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer/Renderer.h"
#include "renderer/Skeletion.h"
#include "animation/Animation.h"
#include "animation/AnimationPlayer.h"

int main()
{
    // --------------------------------------------------------
    // GLFW
    // --------------------------------------------------------

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

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

    GLFWwindow* window =
        glfwCreateWindow(
            1280,
            720,
            "Shadow Fight Renderer",
            nullptr,
            nullptr
        );

    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    // --------------------------------------------------------
    // GLAD
    // --------------------------------------------------------

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }


    std::cout
        << "OpenGL: "
        << glGetString(GL_VERSION)
        << '\n';


    // --------------------------------------------------------
    // Renderer
    // --------------------------------------------------------

    Renderer renderer;

    if (!renderer.initialize())
    {
        std::cerr << "Renderer initialization failed\n";
        return -1;
    }


    // --------------------------------------------------------
    // Mock animation
    // --------------------------------------------------------

    Animation mockAnimation;

    AnimationFrame frame;

    frame.nodes =
    {
        { 0.0f,  0.70f, 0.0f },
        { 0.0f,  0.50f, 0.0f },
        { 0.0f,  0.20f, 0.0f },
        { 0.0f, -0.20f, 0.0f },

        {-0.45f, 0.25f, 0.0f },
        { 0.45f, 0.25f, 0.0f },

        {-0.25f,-0.70f, 0.0f },
        { 0.25f,-0.70f, 0.0f }
    };

    mockAnimation.frameCount = 1;

    mockAnimation.frames.push_back(frame);


    AnimationPlayer player;

    player.setAnimation(&mockAnimation);
    player.play();


    Skeleton skeleton;


    // --------------------------------------------------------
    // Render loop
    // --------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(
            0.05f,
            0.05f,
            0.05f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);


        player.update(1.0f / 60.0f);

        const AnimationFrame* currentFrame =
            player.getCurrentFrame();


        if (currentFrame)
        {
            renderer.renderFrame(
                *currentFrame,
                skeleton
            );
        }


        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}