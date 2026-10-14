#include "PlayerState.h"
#include "CharacterState.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include <thread>
#include <chrono>
#include "CharacterAnimation.h"
#include "animation/AnimationPlayer.h"
#include "animation/loader.h"

PlayerState::PlayerState(GLFWwindow* window, AnimationPlayer* player, CharacterAnimation* characterAnimator)
:
window(window)
{
    this->player = player;
    this->characterAnimator = characterAnimator;
};

CharacterState PlayerState::getState(){
    return currentState.load();
}
bool PlayerState::isStateChanged(){
    CharacterState state = currentState.load();
    if(currentState != IDLE){
        return !player->isPlaying();
    }
    else {
        return isKeyPress();
    }
}

bool PlayerState::isKeyPress(){
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if(currentState.load() != DOUBLE_PUNCH){
            std::cout << "Key Pressed" << std::endl;
            currentState.store(DOUBLE_PUNCH);
            std::cout << "Stored current State" << std::endl;
            characterAnimator->setAnimation(currentState.load());
            return true;
        }
        return false;
    }
    else return false;
}

void PlayerState::trial_thread(std::stop_token stopToken){
    while (!glfwWindowShouldClose(window) && !stopToken.stop_requested()){
        CharacterState state = currentState.load();
        if(state != IDLE){
            if(!player->isPlaying()){
                currentState.store(IDLE);
                AnimationFrame newIdle = createAbsoluteFrame(characterAnimator->getIdle(), *player->getCurrentFrame());
                characterAnimator->setIdle(newIdle);
            }
        }
        else {
            isKeyPress();
        }
    }
      std::cout << "PlayerState thread ended\n";
}