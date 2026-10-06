#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "PlayerState.h"
#include "map"
#include "animation/Animation.h"
#include  "animation/AnimationPlayer.h"
#include "renderer/Skeletion.h"
#include "renderer/Renderer.h"
#include "GLFW/glfw3.h"


class CharacterAnimation{
    public:
        CharacterAnimation(GLFWwindow* window, PlayerState* playerState);
        void updateFrame();
        
        private:
        void setAnimation(CharacterState state);
        void handleIDLE();
        void handleACTIVE();
        
        GLFWwindow* window;
        PlayerState* playerState;
        std::map<CharacterState, Animation> animationMap;
        AnimationPlayer player;
        AnimationFrame idleFrame;
        Skeleton skeleton;
        Renderer renderer;

};