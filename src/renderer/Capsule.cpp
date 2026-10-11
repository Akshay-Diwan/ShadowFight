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
){
    // ------------------------------------------------------------
    // Get capsule length from original node positions
    // ------------------------------------------------------------
    const float& m1 = margin1;
    const float& m2 = margin2;
    const float& hemisphereRadius = radius1;
    const float& cylinderRadius = radius1;

    glm::vec3 originalAxis = p2 - p1;

    float length = glm::length(originalAxis);
    if (length < 0.0001f)
        return;
    // ------------------------------------------------------------
    // LOCAL COORDINATE SYSTEM
    // ------------------------------------------------------------

    glm::vec3 w(0.0f, 1.0f, 0.0f);

    // Choose perpendicular axes
    glm::vec3 u(1.0f, 0.0f, 0.0f);
    glm::vec3 v(0.0f, 0.0f, 1.0f);

    // Local endpoints
    glm::vec3 localP1(
        0.0f,
        -length * 0.5f,
        0.0f
    );

    glm::vec3 localP2(
        0.0f,
        length * 0.5f,
        0.0f
    );

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
        static_cast<unsigned int>(
            mesh.vertices.size()
        );

    glm::vec3 direction =
        localP2 - localP1;

    glm::vec3 center1 =
        localP1 + (m1 * direction);

    glm::vec3 center2 =
        localP2 - (m2 * direction);

    for (int ring = 0; ring <= 1; ring++)
    {
        glm::vec3 center =
            ring == 0 ? center1 : center2;

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
                center +
                radial * cylinderRadius;

            addVertex(position, radial);
        }
    }

    // ------------------------------------------------------------
    // Connect cylinder rings
    // ------------------------------------------------------------

    for (int i = 0; i < radialSegments; i++)
    {
        int next =
            (i + 1) % radialSegments;

        unsigned int a =
            cylinderStart + i;

        unsigned int b =
            cylinderStart + next;

        unsigned int c =
            cylinderStart +
            radialSegments +
            next;

        unsigned int d =
            cylinderStart +
            radialSegments +
            i;

        mesh.indices.push_back(a);
        mesh.indices.push_back(b);
        mesh.indices.push_back(c);

        mesh.indices.push_back(a);
        mesh.indices.push_back(c);
        mesh.indices.push_back(d);
    }

    // ------------------------------------------------------------
    // Hemisphere at local P1
    // ------------------------------------------------------------

    unsigned int bottomStart =
        static_cast<unsigned int>(
            mesh.vertices.size()
        );

    for (int ring = 0;
         ring <= hemisphereRings;
         ring++)
    {
        float phi =
            glm::half_pi<float>() *
            static_cast<float>(ring) /
            static_cast<float>(hemisphereRings);

        float radialAmount =
            std::cos(phi) *
            hemisphereRadius;

        float axialAmount =
            -std::sin(phi) *
            hemisphereRadius;

        for (int i = 0;
             i < radialSegments;
             i++)
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

    // ------------------------------------------------------------
    // Connect bottom hemisphere
    // ------------------------------------------------------------

    for (int ring = 0;
         ring < hemisphereRings;
         ring++)
    {
        for (int i = 0;
             i < radialSegments;
             i++)
        {
            int next =
                (i + 1) % radialSegments;

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
    // Hemisphere at local P2
    // ------------------------------------------------------------

    unsigned int topStart =
        static_cast<unsigned int>(
            mesh.vertices.size()
        );

    for (int ring = 0;
         ring <= hemisphereRings;
         ring++)
    {
        float phi =
            glm::half_pi<float>() *
            static_cast<float>(ring) /
            static_cast<float>(hemisphereRings);

        float radialAmount =
            std::cos(phi) *
            hemisphereRadius;

        float axialAmount =
            std::sin(phi) *
            hemisphereRadius;

        for (int i = 0;
             i < radialSegments;
             i++)
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
                localP2 +
                radial * radialAmount +
                w * axialAmount;

            addVertex(position, normal);
        }
    }

    // ------------------------------------------------------------
    // Connect top hemisphere
    // ------------------------------------------------------------

    for (int ring = 0;
         ring < hemisphereRings;
         ring++)
    {
        for (int i = 0;
             i < radialSegments;
             i++)
        {
            int next =
                (i + 1) % radialSegments;

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

    newAxis = glm::normalize(newAxis);


    glm::vec3 newCenter =
        (newP1 + newP2) * 0.5f;


    glm::vec3 originalAxis(
        0.0f,
        1.0f,
        0.0f
    );

    glm::quat rotation =
        glm::rotation(
            originalAxis,
            newAxis
        );

    // Move capsule to the new center
    transform = glm::mat4(1.0f);
    transform =
        glm::translate(
            transform,
            newCenter
        );
    // Rotate capsule
    transform *= glm::mat4_cast(rotation);
}