#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer/Renderer.h"
#include "renderer/Skeletion.h"
#include "animation/Animation.h"
#include "animation/AnimationPlayer.h"
#include "animation/loader.h"
#include "renderer/BackgroundRenderer.h"

int main()
{
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

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    std::cout
        << "OpenGL: "
        << glGetString(GL_VERSION)
        << '\n';

    Renderer renderer;

    if (!renderer.initialize())
    {
        std::cerr << "Renderer initialization failed\n";
        return -1;
    }
    BackgroundRenderer bgRenderer;
    if(!bgRenderer.initialize()){
        std::cerr << "BackgroundRenderer initialization failed\n";
        return -1;
    }


    Animation animation = loadAnimation("C:/Users/AKSHAY/Desktop/ShadowFight/assets/Double_Punch.bin");
    AnimationPlayer player;

    player.setAnimation(&animation);
    player.play();

    Skeleton skeleton;

    while (!glfwWindowShouldClose(window))
    {
        bgRenderer.render("C:/Users/AKSHAY/Desktop/ShadowFight/assets/background.png");
        // glClearColor(
        //     0.05f,
        //     0.05f,
        //     0.05f,
        //     1.0f
        // );
        // glClear(GL_COLOR_BUFFER_BIT);
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
