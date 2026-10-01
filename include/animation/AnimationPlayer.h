// AnimationPlayer.h

#pragma once

#include "animation/Animation.h"

class AnimationPlayer
{
public:

    AnimationPlayer();

    void setAnimation(const Animation* animation);

    void play();
    void pause();
    void stop();

    void update(float deltaTime);

    const AnimationFrame* getCurrentFrame() const;

private:

    const Animation* animation;

    int currentFrame;

    float frameTimer;

    float frameDuration;

    bool playing;
};