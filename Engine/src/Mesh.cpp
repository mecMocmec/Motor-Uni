#include "Engine/Mesh.h"

#include "Engine/Log.h"
#include "GLInclude.h"

#include <algorithm>
#include <cstddef>
#include <utility>

namespace Engine
{
std::shared_ptr<Mesh> Mesh::Create(std::string name, std::vector<Vertex> vertices, std::vector<std::uint32_t> indices)
{
    return std::shared_ptr<Mesh>(new Mesh(std::move(name), std::move(vertices), std::move(indices)));
}

Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::vector<std::uint32_t> indices)
    : m_name(std::move(name)), m_vertices(std::move(vertices)), m_indices(std::move(indices))
{
    if (!m_vertices.empty())
    {
        m_aabbMin = m_vertices.front().position;
        m_aabbMax = m_vertices.front().position;
        for (const Vertex& vertex : m_vertices)
        {
            m_aabbMin = glm::min(m_aabbMin, vertex.position);
            m_aabbMax = glm::max(m_aabbMax, vertex.position);
        }
    }
    Upload();
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Upload()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_indices.size() * sizeof(std::uint32_t)), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    ENGINE_LOGF("Mesh '{}' uploaded: {} vertices, {} triangles", m_name, m_vertices.size(), GetTriangleCount());
}

void Mesh::Draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
} // namespace Engine
