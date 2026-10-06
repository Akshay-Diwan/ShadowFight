#include "animation/loader.h"
#include "CharacterAnimation.h"
#include "iostream"
CharacterAnimation::CharacterAnimation(GLFWwindow* window, PlayerState* playerState)
: 
playerState(playerState),
window(window)
{
    if (!renderer.initialize())
    {
        std::cerr << "Renderer initialization failed\n";
    }
    Animation animation = loadAnimation("C:/Users/AKSHAY/Desktop/ShadowFight/assets/Double_Punch.bin");
    animationMap[DOUBLE_PUNCH] = animation;
    idleFrame = animation.frames[0];
    std::cout << "Idle nodes: "
          << idleFrame.nodes.size()
          << std::endl;
          
    std::cout << skeleton.bones.size() << std::endl;

};
void CharacterAnimation::setAnimation(CharacterState state){
    auto it = animationMap.find(state);
    if(it == animationMap.end()){
        std::cerr << "Animation not found in map";
        return;
    }
    const Animation& animation = it->second;
    player.setAnimation(&animation);
    player.play();
}

void CharacterAnimation::updateFrame(){
    handleIDLE();
    if(playerState->isStateChanged()){
        setAnimation(playerState->getState());
    }
    CharacterState state = playerState->getState();
    if(state == IDLE){
        handleIDLE();
    }
    else {
        handleACTIVE();
    }
}
void CharacterAnimation::handleIDLE(){
    renderer.renderFrame(
        idleFrame,
        skeleton
    );
}
void CharacterAnimation::handleACTIVE(){
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
}