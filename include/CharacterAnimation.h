#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "CharacterState.h"
#include "map"
#include "animation/Animation.h"
#include  "animation/AnimationPlayer.h"
#include "renderer/Skeletion.h"
#include "renderer/Renderer.h"
#include "GLFW/glfw3.h"


class CharacterAnimation{
    public:
        CharacterAnimation(GLFWwindow* window, AnimationPlayer* player);
        void updateFrame();
        void setIdle(const AnimationFrame& frame);
        void setAnimation(CharacterState state);

        private:
        void handleIDLE();
        void handleACTIVE();
        
        GLFWwindow* window;
        std::map<CharacterState, Animation> animationMap;
        AnimationPlayer* player;
        AnimationFrame idleFrame;
        Skeleton skeleton;
        Renderer renderer;
        CharacterState state;
};