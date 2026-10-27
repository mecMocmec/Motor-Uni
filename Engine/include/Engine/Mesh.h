#pragma once

#include <glm/glm.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Engine
{
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Malla: dades a CPU + buffers a la VRAM. Es comparteix amb shared_ptr.
// Ha de destruir-se mentre el context d'OpenGL encara existeix.
class Mesh
{
public:
    static std::shared_ptr<Mesh> Create(std::string name, std::vector<Vertex> vertices, std::vector<std::uint32_t> indices);

    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void Draw() const;

    const std::string& GetName() const { return m_name; }
    const std::vector<Vertex>& GetVertices() const { return m_vertices; }
    const std::vector<std::uint32_t>& GetIndices() const { return m_indices; }
    std::size_t GetTriangleCount() const { return m_indices.size() / 3; }

    // Caixa envolupant (AABB) en espai local.
    const glm::vec3& GetAabbMin() const { return m_aabbMin; }
    const glm::vec3& GetAabbMax() const { return m_aabbMax; }

private:
    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<std::uint32_t> indices);
    void Upload();

    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<std::uint32_t> m_indices;
    glm::vec3 m_aabbMin{0.0f};
    glm::vec3 m_aabbMax{0.0f};

    std::uint32_t m_vao = 0;
    std::uint32_t m_vbo = 0;
    std::uint32_t m_ebo = 0;
};
} // namespace Engine
