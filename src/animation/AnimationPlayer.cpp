// AnimationPlayer.cpp

#include "animation/AnimationPlayer.h"


AnimationPlayer::AnimationPlayer()
    : animation(nullptr),
      currentFrame(0),
      frameTimer(0.0f),
      frameDuration(1.0f / 30.0f),
      playing(false)
{
}


void AnimationPlayer::setAnimation(
    const Animation* anim)
{
    animation = anim;

    currentFrame = 0;
    frameTimer = 0.0f;
}


void AnimationPlayer::play()
{
    playing = true;
}


void AnimationPlayer::pause()
{
    playing = false;
}


void AnimationPlayer::stop()
{
    playing = false;

    currentFrame = 0;
    frameTimer = 0.0f;
}


void AnimationPlayer::update(float deltaTime)
{
    if (!playing)
        return;

    if (animation == nullptr)
        return;

    if (animation->frameCount <= 0)
        return;


    frameTimer += deltaTime;


    while (frameTimer >= frameDuration)
    {
        frameTimer -= frameDuration;

        currentFrame++;

        if (currentFrame >= animation->frameCount)
        {
            currentFrame = 0;
        }
    }
}


const AnimationFrame*
AnimationPlayer::getCurrentFrame() const
{
    if (animation == nullptr)
        return nullptr;

    if (animation->frames.empty())
        return nullptr;

    return &animation->frames[currentFrame];
}