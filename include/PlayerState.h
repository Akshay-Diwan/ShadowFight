#pragma once
#include "CharacterState.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "thread"
#include "chrono"
#include "stop_token"
#include "animation/AnimationPlayer.h"
#include "CharacterAnimation.h"

class PlayerState{
    public:
    PlayerState(GLFWwindow* window, AnimationPlayer* player, CharacterAnimation* characterAnimator);
    CharacterState getState();
    bool isStateChanged();
    void  trial_thread(std::stop_token stopToken);
    private:
    AnimationPlayer* player;
    CharacterAnimation* characterAnimator;
    bool isKeyPress();
    std::atomic<CharacterState> currentState{IDLE};
    GLFWwindow* window;
};

