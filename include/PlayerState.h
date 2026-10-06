#pragma once
#include "CharacterState.h"
#include "GLFW/glfw3.h"

class PlayerState{
    public:
    PlayerState(GLFWwindow* window);
    CharacterState getState();
    bool isStateChanged();
    
    private:
    bool isKeyPress();
    CharacterState currentState;
    GLFWwindow* window;
};

