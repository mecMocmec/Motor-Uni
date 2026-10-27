#include "Engine/Primitives.h"

#include <array>
#include <cstdint>
#include <vector>

namespace Engine::Primitives
{
std::shared_ptr<Mesh> CreateCube()
{
    // Per a cada cara: normal n i eixos u, v amb u x v = n (triangles antihoraris des de fora).
    struct Face
    {
        glm::vec3 n;
        glm::vec3 u;
        glm::vec3 v;
    };
    const std::array<Face, 6> faces = {{
        {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
        {{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
        {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
        {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
        {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
        {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
    }};

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    vertices.reserve(24);
    indices.reserve(36);

    for (const Face& face : faces)
    {
        const auto base = static_cast<std::uint32_t>(vertices.size());
        const glm::vec3 center = face.n * 0.5f;
        const glm::vec3 u = face.u * 0.5f;
        const glm::vec3 v = face.v * 0.5f;

        vertices.push_back(Vertex{center - u - v, face.n, glm::vec2(0.0f, 0.0f)});
        vertices.push_back(Vertex{center + u - v, face.n, glm::vec2(1.0f, 0.0f)});
        vertices.push_back(Vertex{center + u + v, face.n, glm::vec2(1.0f, 1.0f)});
        vertices.push_back(Vertex{center - u + v, face.n, glm::vec2(0.0f, 1.0f)});

        indices.insert(indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
    }

    return Mesh::Create("Cube", std::move(vertices), std::move(indices));
}
} // namespace Engine::Primitives
