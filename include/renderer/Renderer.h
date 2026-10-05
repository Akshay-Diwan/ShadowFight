#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cstdint>

#include "animation/Animation.h"
#include "renderer/Skeletion.h"
#include "renderer/Shader.h"
#include "string"

struct CapsuleVertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct CapsuleMesh
{
    std::vector<CapsuleVertex> vertices;
    std::vector<unsigned int> indices;
};
class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize();

    void renderFrame(
        const AnimationFrame& frame,
        const Skeleton& skeleton
    );

private:
CapsuleMesh createCapsule(
    const glm::vec3& p1,
    const glm::vec3& p2,
    float cylinderRadius,
    float hemisphereRadius,
    float m1,
    float m2,
    int radialSegments = 8,
    int hemisphereRings = 16
);
GLuint shaderProgram;
GLuint vao;
GLuint vbo;
GLuint ebo;
Shader capsuleShader;

    GLint projectionLocation;
    GLint viewLocation;
    GLint modelLocation;
};