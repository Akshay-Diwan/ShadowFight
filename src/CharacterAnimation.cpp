#include "animation/loader.h"
#include "CharacterAnimation.h"
#include "iostream"
#include "animation/AnimationPlayer.h"
CharacterAnimation::CharacterAnimation(GLFWwindow* window, AnimationPlayer* player)
: 
window(window),
player(player)
{
    setIdle(loadInitialIdleFrame());
    Animation animation = loadAnimation("C:/Users/AKSHAY/Desktop/ShadowFight/assets/Double_Punch.bin", idleFrame);
    animationMap[DOUBLE_PUNCH] = animation;
     std::cout << "Idle nodes: "
          << idleFrame.nodes.size()
          << std::endl;
          
    std::cout << skeleton.bones.size() << std::endl;
    if (!renderer.initialize(idleFrame, skeleton))
    {
        std::cerr << "Renderer initialization failed\n";
    }
};
void CharacterAnimation::setIdle(const AnimationFrame& frame){
    state = IDLE;
    idleFrame = frame;
}
AnimationFrame CharacterAnimation::getIdle(){
    return idleFrame;
}
void CharacterAnimation::setAnimation(CharacterState state){
    this->state = state;
    auto it = animationMap.find(state);
    if(it == animationMap.end()){
        std::cerr << "Animation not found in map";
        return;
    }
    const Animation& animation = it->second;
    player->setAnimation(&animation);
    player->play();
}

void CharacterAnimation::updateFrame(){
    // handleIDLE();
    if(state == IDLE){
        handleIDLE();
    }
    else {
        handleACTIVE();
    }
}
void CharacterAnimation::handleIDLE(){
    // renderer.renderIdleFrame(
    //     skeleton,
    //     idleFrame
    // );
    renderer.renderFrame(
        animationMap[DOUBLE_PUNCH].frames[0],
        skeleton,
        idleFrame
    );
}
void CharacterAnimation::handleACTIVE(){
    player->update(1.0f / 60.0f);
    const AnimationFrame* currentFrame =
        player->getCurrentFrame();
        if (currentFrame)
        {
            renderer.renderFrame(
                *currentFrame,
                skeleton,
                idleFrame
            );
        }
}