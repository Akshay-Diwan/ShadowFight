#include <cstdint>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "animation/loader.h"



AnimationFrame loadInitialIdleFrame()
{
    std::ifstream file(
        "C:/Users/AKSHAY/Desktop/ShadowFight/assets/Double_Punch.bin",
        std::ios::binary
    );

    if (!file)
        throw std::runtime_error("Failed to open initial animation");

    int32_t frameCount = 0;

    file.read(reinterpret_cast<char*>(&frameCount), sizeof(frameCount));

    if (!file || frameCount <= 0)
        throw std::runtime_error("Invalid frame count");

    uint8_t padding = 0;

    file.read(reinterpret_cast<char*>(&padding), sizeof(padding));

    if (!file)
        throw std::runtime_error("Failed to read padding byte");

    int32_t nodeCount = 0;

    file.read(reinterpret_cast<char*>(&nodeCount), sizeof(nodeCount));

    if (!file || nodeCount <= 0 || nodeCount > 10000)
        throw std::runtime_error("Invalid node count");

    AnimationFrame animationFrame;
    animationFrame.nodes.reserve(
        static_cast<std::size_t>(nodeCount)
    );

    for (int32_t node = 0; node < nodeCount; ++node)
    {
        float x, binY, binZ;

        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&binY), sizeof(binY));
        file.read(reinterpret_cast<char*>(&binZ), sizeof(binZ));

        if (!file)
            throw std::runtime_error(
                "Unexpected end of animation file at node " +
                std::to_string(node)
            );

        NodePosition position;
        position.x = x;
        position.y = -binZ;
        position.z = binY;

        animationFrame.nodes.push_back(position);
    }

    return animationFrame;
}
AnimationFrame createAbsoluteFrame(const AnimationFrame& idleFrame, const AnimationFrame& deltaFrame){
    AnimationFrame absFrame;
    absFrame.nodes.reserve(idleFrame.nodes.size());
    for(int i = 0; i < idleFrame.nodes.size(); i++){
        NodePosition position;
        position.x = idleFrame.nodes[i].x + deltaFrame.nodes[i].x;
        position.y = idleFrame.nodes[i].y + deltaFrame.nodes[i].y;
        position.z = idleFrame.nodes[i].z + deltaFrame.nodes[i].z;
        absFrame.nodes.push_back(position);
    }
    return absFrame;
}
Animation loadAnimation(const std::string& filename, const AnimationFrame& idleFrame)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
        throw std::runtime_error("Failed to open animation: " + filename);

    Animation animation;

    // frameCount
    file.read(
        reinterpret_cast<char*>(&animation.frameCount),
        sizeof(int32_t)
    );

    if (!file)
        throw std::runtime_error("Failed to read frame count");

    animation.frames.reserve(animation.frameCount);

    for (int frame = 0; frame < animation.frameCount; ++frame)
    {
        // Padding byte
        uint8_t padding;

        file.read(
            reinterpret_cast<char*>(&padding),
            sizeof(uint8_t)
        );

        // Number of nodes
        int32_t nodeCount;

        file.read(
            reinterpret_cast<char*>(&nodeCount),
            sizeof(int32_t)
        );

        if (!file)
            throw std::runtime_error("Failed to read node count");

        AnimationFrame animationFrame;
        animationFrame.nodes.reserve(nodeCount);

        for (int node = 0; node < nodeCount; ++node)
        {
            float x;
            float binY;
            float binZ;

            file.read(reinterpret_cast<char*>(&x), sizeof(float));
            file.read(reinterpret_cast<char*>(&binY), sizeof(float));
            file.read(reinterpret_cast<char*>(&binZ), sizeof(float));

            if (!file)
                throw std::runtime_error("Unexpected end of animation file");

            NodePosition position;

            // BIN stores:
            // x, z, -y
            //
            // Convert back to Blender coordinates:
            // x, y, z

            position.x = x - idleFrame.nodes[node].x;
            position.y = -binZ - idleFrame.nodes[node].y;
            position.z = binY - idleFrame.nodes[node].z;

            animationFrame.nodes.push_back(position);
        }

        animation.frames.push_back(std::move(animationFrame));
    }

    return animation;
}