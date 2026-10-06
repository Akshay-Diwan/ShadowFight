#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "renderer/BackgroundRenderer.h"
#include "CharacterAnimation.h"

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


    
    BackgroundRenderer bgRenderer;
    if(!bgRenderer.initialize()){
        std::cerr << "BackgroundRenderer initialization failed\n";
        return -1;
    }


    // Animation animation = loadAnimation("C:/Users/AKSHAY/Desktop/ShadowFight/assets/Double_Punch.bin");
    // AnimationPlayer player;

    // player.setAnimation(&animation);
    // player.play();

    // Skeleton skeleton;
    CharacterAnimation playerAnimation(window);
    while (!glfwWindowShouldClose(window))
    {
        bgRenderer.render("C:/Users/AKSHAY/Desktop/ShadowFight/assets/background.png");
        playerAnimation.updateFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}