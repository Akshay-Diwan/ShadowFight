#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include <iostream>
#include "renderer/Figure.h"
#include <memory>

Renderer::Renderer()
    : projectionLocation(-1),
      viewLocation(-1),
      modelLocation(-1),
      capsuleShader("capsule")
{
}

Renderer::~Renderer()
{
}

bool Renderer::initialize(const AnimationFrame& idleFrame, const Skeleton& skeleton)
{
    capsuleShader.use();
    glm::vec3 cameraPos(
         280.0f, -900.0f, 280.0f
     );
     glm::vec3 target(
         280.0f,
         -70.0f,
         280.0f
     );
     glm::vec3 up(
         0.0f,
         0.0f,
         1.0f
     );

    glm::mat4 view = glm::lookAt(
        cameraPos,
        target,
        up
    );

    float width = 800.0f;
    float height = 600.0f;

    glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            width / height,
            0.1f,
            2000.0f
    );
    glm::mat4 model = glm::mat4(1.0f);

    capsuleShader.setMat4("uView", view);
    capsuleShader.setMat4("uProjection", projection);
    capsuleShader.setMat4("uModel", model);
    initialRender(idleFrame, skeleton);
    return true;
}
void Renderer::initialRender(
    const AnimationFrame& idleFrame,
     const Skeleton& skeleton
){
    capsuleShader.use();
    for(Capsule& capsule: capsules){
        auto it = skeleton.bones.find(capsule.edgeName);
        if(it == skeleton.bones.end()) continue;
        std::cout << "Fetch node position" << std::endl;
        const NodePosition& node1 = idleFrame.nodes[it->second[0]];
        const NodePosition& node2 = idleFrame.nodes[it->second[1]];
        std::cout << "node position fetched!" << std::endl;

        glm::vec3 p1(node1.x, node1.y, node1.z);
        glm::vec3 p2(node2.x, node2.y, node2.z);
        capsule.createMesh(p1, p2);
        std::cout << "Mesh created!" << std::endl;
        capsule.initializeBuffers();
    }
}
void Renderer::renderIdleFrame(
    const Skeleton& skeleton,
    const AnimationFrame& idleFrame
){
    AnimationFrame zeroFrame;
    zeroFrame.nodes.reserve(idleFrame.nodes.size());
    for(int i = 0; i < idleFrame.nodes.size(); i++){
        NodePosition temp;
        temp.x = 0; temp.y = 0; temp.z = 0;
        zeroFrame.nodes.push_back(temp);
    }
    renderFrame(zeroFrame, skeleton, idleFrame);
}
void Renderer::renderFrame(
    const AnimationFrame& frame,
    const Skeleton& skeleton,
    const AnimationFrame& idleFrame)
{
    capsuleShader.use();
    for (Capsule& capsule : capsules)
    {
        auto it = skeleton.bones.find(capsule.edgeName);
        if (it == skeleton.bones.end()){
            continue;
        }

        const NodePosition& node1 =
            frame.nodes[it->second[0]];
        const NodePosition& deltaNode1 =
            idleFrame.nodes[it->second[0]];

        const NodePosition& node2 =
            frame.nodes[it->second[1]];
        const NodePosition& deltaNode2 =
            idleFrame.nodes[it->second[1]];

        glm::vec3 p1(node1.x + deltaNode1.x, node1.y + deltaNode1.y, node1.z + deltaNode1.z);
        glm::vec3 p2(node2.x + deltaNode2.x, node2.y + deltaNode2.y, node2.z + deltaNode2.z);

        capsule.transformMesh(p1, p2);
        capsuleShader.setMat4("uModel", capsule.transform);

        glBindVertexArray(capsule.mesh.VAO);
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(capsule.mesh.indices.size()),
            GL_UNSIGNED_INT,
            nullptr
        );
    }

    glBindVertexArray(0);
}



