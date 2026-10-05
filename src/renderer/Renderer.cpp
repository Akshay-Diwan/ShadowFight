#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include <iostream>
#include "renderer/Figure.h"

// ============================================================
// Constructor
// ============================================================
Renderer::Renderer()
    : vao(0),
      vbo(0),
      projectionLocation(-1),
      viewLocation(-1),
      modelLocation(-1),
      capsuleShader("capsule")
{
}


// ============================================================
// Destructor
// ============================================================

Renderer::~Renderer()
{
    if (vbo != 0)
        glDeleteBuffers(1, &vbo);

    if (vao != 0)
        glDeleteVertexArrays(1, &vao);
}

bool Renderer::initialize()
{
    capsuleShader.use();
   glm::vec3 cameraPos(
        280.0f, -600.0f, 120.0f
    );
    glm::vec3 target(
        280.0f,
        -70.0f,
        120.0f
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
    glGenVertexArrays(
        1,
        &vao
    );
    glBindVertexArray(vao);
    glGenBuffers(
        1,
        &vbo
    );
    glGenBuffers(
        1, &ebo
    );
    glBindBuffer(
        GL_ARRAY_BUFFER,
        vbo
    );
    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        ebo
    );
    // We will update this every frame
    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, nullptr,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CapsuleVertex),
        (void*)offsetof(CapsuleVertex, position)
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(CapsuleVertex),
        (void*)offsetof(CapsuleVertex, normal)
    );

    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return true;
}

//Render Frame 
void Renderer::renderFrame(
    const AnimationFrame& frame,
    const Skeleton& skeleton
)
{
    capsuleShader.setVec3("uColor", 1.0f, 1.0f, 1.0f);
    glBindVertexArray(vao);
    
    
    for(Capsule capsule: capsules){
        auto it = skeleton.bones.find(capsule.edgeName);
        if(it != skeleton.bones.end()){
            NodePosition node1 = frame.nodes[it->second[0]];
            glm::vec3 p1(node1.x,node1.y,node1.z);
            NodePosition node2 = frame.nodes[it->second[1]];
            glm::vec3 p2(node2.x, node2.y, node2.z);
            CapsuleMesh capsuleMesh = createCapsule(
                p1,
                p2,
                capsule.radius1, capsule.radius1,
                capsule.margin1, capsule.margin2
            );
                    // Upload vertices
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(
            GL_ARRAY_BUFFER,
            capsuleMesh.vertices.size() * sizeof(CapsuleVertex),
            capsuleMesh.vertices.data(),
            GL_DYNAMIC_DRAW
        );

        // Upload indices
        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            ebo
        );

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            capsuleMesh.indices.size() * sizeof(unsigned int),
            capsuleMesh.indices.data(),
            GL_DYNAMIC_DRAW
        );
    glDisable(GL_DEPTH_TEST);
            glDrawElements(
               GL_TRIANGLES,
               static_cast<GLsizei>(capsuleMesh.indices.size()),
               GL_UNSIGNED_INT,
               nullptr
             ) ;
        }
       
    }

    glBindVertexArray(0);
}

CapsuleMesh Renderer::createCapsule(
    const glm::vec3& p1,
    const glm::vec3& p2,
    float cylinderRadius,
    float hemisphereRadius,
    float m1,
    float m2,
    int radialSegments,
    int hemisphereRings
)
{
    CapsuleMesh mesh;
    glm::vec3 axis = p2 - p1;
    float length = glm::length(axis);
    if (length < 0.0001f)
        return mesh;

    glm::vec3 w = glm::normalize(axis);

    glm::vec3 temp;

    if (std::abs(w.x) < 0.9f)
        temp = glm::vec3(1, 0, 0);
    else
        temp = glm::vec3(0, 1, 0);

    glm::vec3 u = glm::normalize(glm::cross(w, temp));
    glm::vec3 v = glm::cross(w, u);

    // ------------------------------------------------------------
    // Helper: add a vertex
    // ------------------------------------------------------------

    auto addVertex =
        [&](const glm::vec3& position,
            const glm::vec3& normal)
    {
        mesh.vertices.push_back({
            position,
            glm::normalize(normal)
        });
    };

    // ------------------------------------------------------------
    // Cylinder
    // ------------------------------------------------------------

    unsigned int cylinderStart =
        static_cast<unsigned int>(mesh.vertices.size());

    glm::vec3 direction = p2 - p1;
    glm::vec3 center1 = p1 + (m1 * direction);
    glm::vec3 center2 = p2 - (m2 * direction);
    for (int ring = 0; ring <= 1; ring++)
    {
        glm::vec3 center = ring == 0 ? center1 : center2;
        for (int i = 0; i < radialSegments; i++)
        {
            float theta =
                2.0f * glm::pi<float>() *
                static_cast<float>(i) /
                static_cast<float>(radialSegments);

            glm::vec3 radial =
                std::cos(theta) * u +
                std::sin(theta) * v;

            glm::vec3 position =
                center + radial * cylinderRadius;

            addVertex(position, radial);
        }
    }

    // Connect cylinder rings
    for (int i = 0; i < radialSegments; i++)
    {
        int next = (i + 1) % radialSegments;

        unsigned int a =
            cylinderStart + i;

        unsigned int b =
            cylinderStart + next;

        unsigned int c =
            cylinderStart + radialSegments + next;

        unsigned int d =
            cylinderStart + radialSegments + i;

        mesh.indices.push_back(a);
        mesh.indices.push_back(b);
        mesh.indices.push_back(c);

        mesh.indices.push_back(a);
        mesh.indices.push_back(c);
        mesh.indices.push_back(d);
    }

    // ------------------------------------------------------------
    // Hemisphere at p1
    // ------------------------------------------------------------

    unsigned int bottomStart =
        static_cast<unsigned int>(mesh.vertices.size());

    for (int ring = 0; ring <= hemisphereRings; ring++)
    {
        float phi =
            (glm::half_pi<float>() *
             static_cast<float>(ring) /
             static_cast<float>(hemisphereRings));

        float radialAmount =
            std::cos(phi) * hemisphereRadius;

        float axialAmount =
            -std::sin(phi) * hemisphereRadius;

        for (int i = 0; i < radialSegments; i++)
        {
            float theta =
                2.0f * glm::pi<float>() *
                static_cast<float>(i) /
                static_cast<float>(radialSegments);

            glm::vec3 radial =
                std::cos(theta) * u +
                std::sin(theta) * v;

            glm::vec3 normal =
                radial * std::cos(phi) -
                w * std::sin(phi);

            glm::vec3 position =
                center1 +
                radial * radialAmount +
                w * axialAmount;

            addVertex(position, normal);
        }
    }

    // Connect bottom hemisphere rings
    for (int ring = 0; ring < hemisphereRings; ring++)
    {
        for (int i = 0; i < radialSegments; i++)
        {
            int next = (i + 1) % radialSegments;

            unsigned int a =
                bottomStart +
                ring * radialSegments +
                i;

            unsigned int b =
                bottomStart +
                ring * radialSegments +
                next;

            unsigned int c =
                bottomStart +
                (ring + 1) * radialSegments +
                next;

            unsigned int d =
                bottomStart +
                (ring + 1) * radialSegments +
                i;

            mesh.indices.push_back(a);
            mesh.indices.push_back(b);
            mesh.indices.push_back(c);

            mesh.indices.push_back(a);
            mesh.indices.push_back(c);
            mesh.indices.push_back(d);
        }
    }

    // ------------------------------------------------------------
    // Hemisphere at p2
    // ------------------------------------------------------------

    unsigned int topStart =
        static_cast<unsigned int>(mesh.vertices.size());

    for (int ring = 0; ring <= hemisphereRings; ring++)
    {
        float phi =
            (glm::half_pi<float>() *
             static_cast<float>(ring) /
             static_cast<float>(hemisphereRings));

        float radialAmount =
            std::cos(phi) * hemisphereRadius;

        float axialAmount =
            std::sin(phi) * hemisphereRadius;

        for (int i = 0; i < radialSegments; i++)
        {
            float theta =
                2.0f * glm::pi<float>() *
                static_cast<float>(i) /
                static_cast<float>(radialSegments);

            glm::vec3 radial =
                std::cos(theta) * u +
                std::sin(theta) * v;

            glm::vec3 normal =
                radial * std::cos(phi) +
                w * std::sin(phi);

            glm::vec3 position =
                p2 +
                radial * radialAmount +
                w * axialAmount;

            addVertex(position, normal);
        }
    }

    // Connect top hemisphere rings
    for (int ring = 0; ring < hemisphereRings; ring++)
    {
        for (int i = 0; i < radialSegments; i++)
        {
            int next = (i + 1) % radialSegments;

            unsigned int a =
                topStart +
                ring * radialSegments +
                i;

            unsigned int b =
                topStart +
                ring * radialSegments +
                next;

            unsigned int c =
                topStart +
                (ring + 1) * radialSegments +
                next;

            unsigned int d =
                topStart +
                (ring + 1) * radialSegments +
                i;

            mesh.indices.push_back(a);
            mesh.indices.push_back(c);
            mesh.indices.push_back(b);

            mesh.indices.push_back(a);
            mesh.indices.push_back(d);
            mesh.indices.push_back(c);
        }
    }

    return mesh;
}