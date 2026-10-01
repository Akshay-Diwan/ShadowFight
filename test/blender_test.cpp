#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

int main()
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        "../assets/Double_Punch.glb",
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene)
    {
        std::cerr << "Failed to load GLB:\n";
        std::cerr << importer.GetErrorString() << '\n';

        return 1;
    }

    std::cout << "GLB loaded successfully!\n\n";

    std::cout << "Meshes: "
              << scene->mNumMeshes
              << '\n';

    std::cout << "Materials: "
              << scene->mNumMaterials
              << '\n';

    std::cout << "Animations: "
              << scene->mNumAnimations
              << "\n\n";

    for (unsigned int i = 0;
         i < scene->mNumAnimations;
         ++i)
    {
        const aiAnimation* animation =
            scene->mAnimations[i];

        std::cout << "Animation " << i << '\n';

        std::cout << "Name: "
                  << animation->mName.C_Str()
                  << '\n';

        std::cout << "Duration: "
                  << animation->mDuration
                  << '\n';

        std::cout << "Ticks/second: "
                  << animation->mTicksPerSecond
                  << '\n';

        std::cout << "Channels: "
                  << animation->mNumChannels
                  << "\n\n";
    }

    return 0;
}