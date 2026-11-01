#include "Engine/Primitives.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

namespace Engine::Primitives
{
std::shared_ptr<Mesh> CreateCube()
{
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
namespace
{
constexpr float kPi = 3.14159265358979323846f;

void AddFlatTriangle(std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
                     const glm::vec2& uvA, const glm::vec2& uvB, const glm::vec2& uvC)
{
    const glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
    const auto base = static_cast<std::uint32_t>(vertices.size());
    vertices.push_back(Vertex{a, normal, uvA});
    vertices.push_back(Vertex{b, normal, uvB});
    vertices.push_back(Vertex{c, normal, uvC});
    indices.insert(indices.end(), {base, base + 1, base + 2});
}
}

std::shared_ptr<Mesh> CreateSphere()
{
    constexpr int kStacks = 16;
    constexpr int kSectors = 32;
    constexpr float kRadius = 0.5f;

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    vertices.reserve(static_cast<std::size_t>(kStacks + 1) * (kSectors + 1));
    indices.reserve(static_cast<std::size_t>(kStacks) * kSectors * 6);

    for (int i = 0; i <= kStacks; ++i)
    {
        const float theta = kPi * static_cast<float>(i) / static_cast<float>(kStacks);
        for (int j = 0; j <= kSectors; ++j)
        {
            const float phi = 2.0f * kPi * static_cast<float>(j) / static_cast<float>(kSectors);
            const glm::vec3 normal(std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi));
            const glm::vec2 uv(static_cast<float>(j) / static_cast<float>(kSectors), 1.0f - static_cast<float>(i) / static_cast<float>(kStacks));
            vertices.push_back(Vertex{normal * kRadius, normal, uv});
        }
    }

    for (int i = 0; i < kStacks; ++i)
    {
        for (int j = 0; j < kSectors; ++j)
        {
            const auto a = static_cast<std::uint32_t>(i * (kSectors + 1) + j);
            const auto b = static_cast<std::uint32_t>((i + 1) * (kSectors + 1) + j);
            indices.insert(indices.end(), {a, a + 1, b, a + 1, b + 1, b});
        }
    }

    return Mesh::Create("Sphere", std::move(vertices), std::move(indices));
}

std::shared_ptr<Mesh> CreatePyramid()
{
    const glm::vec3 b0(-0.5f, -0.5f, 0.5f);
    const glm::vec3 b1(0.5f, -0.5f, 0.5f);
    const glm::vec3 b2(0.5f, -0.5f, -0.5f);
    const glm::vec3 b3(-0.5f, -0.5f, -0.5f);
    const glm::vec3 apex(0.0f, 0.5f, 0.0f);

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    vertices.reserve(18);
    indices.reserve(18);

    const glm::vec2 uvLeft(0.0f, 0.0f);
    const glm::vec2 uvRight(1.0f, 0.0f);
    const glm::vec2 uvTop(0.5f, 1.0f);
    AddFlatTriangle(vertices, indices, b0, b1, apex, uvLeft, uvRight, uvTop);
    AddFlatTriangle(vertices, indices, b1, b2, apex, uvLeft, uvRight, uvTop);
    AddFlatTriangle(vertices, indices, b2, b3, apex, uvLeft, uvRight, uvTop);
    AddFlatTriangle(vertices, indices, b3, b0, apex, uvLeft, uvRight, uvTop);
    AddFlatTriangle(vertices, indices, b0, b3, b2, glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f));
    AddFlatTriangle(vertices, indices, b0, b2, b1, glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f));

    return Mesh::Create("Pyramid", std::move(vertices), std::move(indices));
}

std::shared_ptr<Mesh> CreateCylinder()
{
    constexpr int kSegments = 32;
    constexpr float kRadius = 0.5f;
    constexpr float kHalfHeight = 0.5f;

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    for (int j = 0; j <= kSegments; ++j)
    {
        const float phi = 2.0f * kPi * static_cast<float>(j) / static_cast<float>(kSegments);
        const float c = std::cos(phi);
        const float s = std::sin(phi);
        const glm::vec3 normal(c, 0.0f, s);
        const float u = static_cast<float>(j) / static_cast<float>(kSegments);
        vertices.push_back(Vertex{glm::vec3(kRadius * c, -kHalfHeight, kRadius * s), normal, glm::vec2(u, 0.0f)});
        vertices.push_back(Vertex{glm::vec3(kRadius * c, kHalfHeight, kRadius * s), normal, glm::vec2(u, 1.0f)});
    }
    for (int j = 0; j < kSegments; ++j)
    {
        const auto bottom = static_cast<std::uint32_t>(2 * j);
        const auto top = bottom + 1;
        const auto nextBottom = static_cast<std::uint32_t>(2 * (j + 1));
        const auto nextTop = nextBottom + 1;
        indices.insert(indices.end(), {bottom, top, nextBottom, top, nextTop, nextBottom});
    }

    for (const bool isTop : {true, false})
    {
        const float y = isTop ? kHalfHeight : -kHalfHeight;
        const glm::vec3 normal(0.0f, isTop ? 1.0f : -1.0f, 0.0f);

        const auto center = static_cast<std::uint32_t>(vertices.size());
        vertices.push_back(Vertex{glm::vec3(0.0f, y, 0.0f), normal, glm::vec2(0.5f, 0.5f)});
        for (int j = 0; j <= kSegments; ++j)
        {
            const float phi = 2.0f * kPi * static_cast<float>(j) / static_cast<float>(kSegments);
            const float c = std::cos(phi);
            const float s = std::sin(phi);
            vertices.push_back(Vertex{glm::vec3(kRadius * c, y, kRadius * s), normal, glm::vec2(0.5f + 0.5f * c, 0.5f + 0.5f * s)});
        }
        for (int j = 0; j < kSegments; ++j)
        {
            const auto ring = center + 1 + static_cast<std::uint32_t>(j);
            if (isTop)
            {
                indices.insert(indices.end(), {center, ring + 1, ring});
            }
            else
            {
                indices.insert(indices.end(), {center, ring, ring + 1});
            }
        }
    }

    return Mesh::Create("Cylinder", std::move(vertices), std::move(indices));
}
}
