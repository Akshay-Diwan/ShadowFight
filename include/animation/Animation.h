#pragma once

#include <cstdint>
#include <vector>

struct NodePosition
{
    float x;
    float y;
    float z;
};

struct AnimationFrame
{
    std::vector<NodePosition> nodes;
};

struct Animation
{
    int32_t frameCount;
    std::vector<AnimationFrame> frames;
};