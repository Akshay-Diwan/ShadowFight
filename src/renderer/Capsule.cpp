#include "renderer/Capsule.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Capsule::Capsule(
    const std::string& edgeName,
    float radius1,
    float radius2,
    float margin1,
    float margin2
)
    : edgeName(edgeName),
      radius1(radius1),
      radius2(radius2),
      margin1(margin1),
      margin2(margin2)
{
}

Capsule::~Capsule()
{
}

void Capsule::initializeBuffers()
{
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.vertices.size() * sizeof(CapsuleVertex),
        mesh.vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.indices.size() * sizeof(unsigned int),
        mesh.indices.data(),
        GL_STATIC_DRAW
    );
    // Assuming Vertex contains position at location 0
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
}

void Capsule::createMesh(
    const glm::vec3& p1,
    const glm::vec3& p2,
    int radialSegments,
    int hemisphereRings
)
{
    if (radialSegments < 3 || hemisphereRings < 1)
        return;
    idleP1 = p1;
    idleP2 = p2;
    const float hemisphereRadius = radius1;
    const float cylinderRadius = radius1;

    // ------------------------------------------------------------
    // Actual world-space center and axis
    // ------------------------------------------------------------

    glm::vec3 direction = p2 - p1;
    float length = glm::length(direction);

    if (length < 0.0001f)
        return;

    mesh.idleCenter = (p1 + p2) * 0.5f;
    mesh.idleAxis = direction / length;

    // ------------------------------------------------------------
    // Build perpendicular vectors around the actual axis
    // ------------------------------------------------------------

    glm::vec3 reference =
        (std::abs(glm::dot(mesh.idleAxis, glm::vec3(0, 1, 0))) < 0.99f)
        ? glm::vec3(0, 1, 0)
        : glm::vec3(1, 0, 0);

    glm::vec3 u =
        glm::normalize(glm::cross(reference, mesh.idleAxis));

    glm::vec3 v =
        glm::normalize(glm::cross(mesh.idleAxis, u));

    // ------------------------------------------------------------
    // Cylinder endpoints: actual coordinates
    // ------------------------------------------------------------

    glm::vec3 center1 = p1 + (margin1 * direction);
    glm::vec3 center2 = p2 - (margin2 * direction);

    auto addVertex = [&](const glm::vec3& position,
                         const glm::vec3& normal)
    {
        mesh.vertices.push_back({
            position,
            glm::normalize(normal)
        });
    };

    // ------------------------------------------------------------
    // Cylinder rings
    // ------------------------------------------------------------

    unsigned int cylinderStart =
        static_cast<unsigned int>(mesh.vertices.size());

    for (int ring = 0; ring <= 1; ++ring)
    {
        glm::vec3 center = (ring == 0) ? center1 : center2;

        for (int i = 0; i < radialSegments; ++i)
        {
            float theta =
                2.0f * glm::pi<float>() * i / radialSegments;

            glm::vec3 radial =
                std::cos(theta) * u +
                std::sin(theta) * v;

            glm::vec3 position =
                center + radial * cylinderRadius;

            addVertex(position, radial);
        }
    }

    for (int i = 0; i < radialSegments; ++i)
    {
        int next = (i + 1) % radialSegments;

        unsigned int a = cylinderStart + i;
        unsigned int b = cylinderStart + next;
        unsigned int c = cylinderStart + radialSegments + next;
        unsigned int d = cylinderStart + radialSegments + i;

        mesh.indices.insert(mesh.indices.end(),
            { a, b, c, a, c, d });
    }

    // ------------------------------------------------------------
    // Hemisphere at p1, extending opposite to the axis
    // ------------------------------------------------------------

    unsigned int bottomStart =
        static_cast<unsigned int>(mesh.vertices.size());

    for (int ring = 0; ring <= hemisphereRings; ++ring)
    {
        float phi =
            glm::half_pi<float>() * ring / hemisphereRings;

        float r = std::cos(phi) * hemisphereRadius;
        float axial = -std::sin(phi) * hemisphereRadius;

        for (int i = 0; i < radialSegments; ++i)
        {
            float theta =
                2.0f * glm::pi<float>() * i / radialSegments;

            glm::vec3 radial =
                std::cos(theta) * u +
                std::sin(theta) * v;

            glm::vec3 normal =
                radial * std::cos(phi) -
                mesh.idleAxis * std::sin(phi);

            glm::vec3 position =
                center1 + radial * r + mesh.idleAxis * axial;

            addVertex(position, normal);
        }
    }

    for (int ring = 0; ring < hemisphereRings; ++ring)
    {
        for (int i = 0; i < radialSegments; ++i)
        {
            int next = (i + 1) % radialSegments;

            unsigned int a =
                bottomStart + ring * radialSegments + i;
            unsigned int b =
                bottomStart + ring * radialSegments + next;
            unsigned int c =
                bottomStart + (ring + 1) * radialSegments + next;
            unsigned int d =
                bottomStart + (ring + 1) * radialSegments + i;

            mesh.indices.insert(mesh.indices.end(),
                { a, b, c, a, c, d });
        }
    }

    // ------------------------------------------------------------
    // Hemisphere at p2, extending along the axis
    // ------------------------------------------------------------

    unsigned int topStart =
        static_cast<unsigned int>(mesh.vertices.size());

    for (int ring = 0; ring <= hemisphereRings; ++ring)
    {
        float phi =
            glm::half_pi<float>() * ring / hemisphereRings;

        float r = std::cos(phi) * hemisphereRadius;
        float axial = std::sin(phi) * hemisphereRadius;

        for (int i = 0; i < radialSegments; ++i)
        {
            float theta =
                2.0f * glm::pi<float>() * i / radialSegments;

            glm::vec3 radial =
                std::cos(theta) * u +
                std::sin(theta) * v;

            glm::vec3 normal =
                radial * std::cos(phi) +
                mesh.idleAxis * std::sin(phi);

            glm::vec3 position =
                center2 + radial * r + mesh.idleAxis * axial;

            addVertex(position, normal);
        }
    }

    for (int ring = 0; ring < hemisphereRings; ++ring)
    {
        for (int i = 0; i < radialSegments; ++i)
        {
            int next = (i + 1) % radialSegments;

            unsigned int a =
                topStart + ring * radialSegments + i;
            unsigned int b =
                topStart + ring * radialSegments + next;
            unsigned int c =
                topStart + (ring + 1) * radialSegments + next;
            unsigned int d =
                topStart + (ring + 1) * radialSegments + i;

            mesh.indices.insert(mesh.indices.end(),
                { a, c, b, a, d, c });
        }
    }
}



void Capsule::transformMesh(
    const glm::vec3& newP1,
    const glm::vec3& newP2
)
{
    glm::vec3 newAxis = newP2 - newP1;
    float length = glm::length(newAxis);

    if (length < 0.0001f)
        return;

    newAxis /= length;

    glm::vec3 newCenter = (newP1 + newP2) * 0.5f;

    // Rotation from the original axis to the new axis
    glm::quat rotation = glm::rotation(
        mesh.idleAxis,
        newAxis
    );

    glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

    // Rotate around the original center, then move to newCenter
    transform =
        glm::translate(glm::mat4(1.0f), newCenter) *
        rotationMatrix *
        glm::translate(glm::mat4(1.0f), -mesh.idleCenter);
}
