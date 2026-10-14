#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cstdint>

#include "animation/Animation.h"
#include "renderer/Skeletion.h"
#include "renderer/Shader.h"
#include "string"
#include "renderer/Figure.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize(
        const AnimationFrame& frame,
        const Skeleton& skeleton
    );
    void renderIdleFrame(
        const Skeleton& skeletion,
        const AnimationFrame& idleFrame
    );
    void renderFrame(
        const AnimationFrame& frame,
        const Skeleton& skeleton,
        const AnimationFrame& idleFrame
    );
    void initialRender(
        const AnimationFrame& idleFrame,
        const Skeleton& skeleton
    );


private:
GLuint shaderProgram;
Shader capsuleShader;
GLint projectionLocation;
GLint viewLocation;
GLint modelLocation;
};