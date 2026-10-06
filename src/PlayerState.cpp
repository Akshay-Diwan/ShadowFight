#include "PlayerState.h"
#include "CharacterState.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include <thread>
#include <chrono>
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return true;
    }
    else false;
}
void PlayerState::trial_thread(std::stop_token stopToken){
    
    while (!glfwWindowShouldClose(window) && !stopToken.stop_requested()){
        isKeyPress();
    }
}