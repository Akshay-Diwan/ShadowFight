// Skeleton.h
#pragma once

#include "animation/CharacterNodes.h"
#include <vector>
#include "map"
#include "string"

struct Bone
{
    NodeId start;
    NodeId end;
};

class Skeleton
{
public:
    std::map<std::string, std::vector<int>> bones;

    Skeleton();
};