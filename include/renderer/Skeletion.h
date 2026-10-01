// Skeleton.h
#pragma once

#include "animation/CharacterNodes.h"
#include <vector>

struct Bone
{
    NodeId start;
    NodeId end;
};

class Skeleton
{
public:
    std::vector<Bone> bones;

    Skeleton();
};