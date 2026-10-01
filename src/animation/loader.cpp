#include <cstdint>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "animation/loader.h"

Animation loadAnimation(const std::string& filename)
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

            position.x = x;
            position.y = -binZ;
            position.z = binY;

            animationFrame.nodes.push_back(position);
        }

        animation.frames.push_back(std::move(animationFrame));
    }

    return animation;
}