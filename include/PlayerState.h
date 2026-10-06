#pragma once
#include "CharacterState.h"
#include "GLFW/glfw3.h"
#include "thread"
#include "chrono"
#include "stop_token"

class PlayerState{
    public:
    PlayerState(GLFWwindow* window);
    CharacterState getState();
    bool isStateChanged();
    void  trial_thread(std::stop_token stopToken);
    private:
    bool isKeyPress();
    std::atomic<CharacterState> currentState{IDLE};
    GLFWwindow* window;
};

