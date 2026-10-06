#include "PlayerState.h"
#include "CharacterState.h"
#include "GLFW/glfw3.h"
#include "iostream"
PlayerState::PlayerState(GLFWwindow* window)
:
currentState(IDLE),
window(window)
{};

CharacterState PlayerState::getState(){
    return currentState;
}
bool PlayerState::isStateChanged(){
    return isKeyPress();
}
bool PlayerState::isKeyPress(){
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        std::cout << "Key Pressed" << std::endl;
        currentState = DOUBLE_PUNCH;
        return true;
    }
    else false;
}