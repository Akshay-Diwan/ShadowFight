#pragma once
#include "glad/gl.h"
#include <glm/glm.hpp>
#include "string"

#include <map>
#include <string>
#include <vector>

struct CapsuleVertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct CapsuleMesh
{
    std::vector<CapsuleVertex> vertices;
    std::vector<unsigned int> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 idleAxis;
    glm::vec3 idleCenter;
};

class Capsule
{
public:
    glm::vec3 idleP1;
    glm::vec3 idleP2;
    float radius1;
    float radius2;

    std::string edgeName;

    float margin1;
    float margin2;
    CapsuleMesh mesh;
    glm::mat4 transform;

public:
    Capsule(
        const std::string& edgeName,
        float radius1,
        float radius2,
        float margin1,
        float margin2
    );

    ~Capsule();

    // Generate mesh geometry
    void createMesh(
        const glm::vec3& p1,
        const glm::vec3& p2,
        int radialSegments = 16,
        int hemisphereRings = 8
    );

    // Initialize VAO/VBO/EBO
    void initializeBuffers();

    // Transform existing mesh
    void transformMesh(
        const glm::vec3& newP1,
        const glm::vec3& newP2
    );
};