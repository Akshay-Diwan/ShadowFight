#pragma once
#include <map>
#include <string>
#include <vector>


struct Capsule {
    std::string edgeName;
    float radius1;
    float radius2;
    float margin1;
    float margin2;
};

extern std::vector<Capsule> capsules;