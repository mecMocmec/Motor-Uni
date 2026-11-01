#include "Engine/Mesh.h"

#include "Engine/Log.h"
#include "Engine/ResourceStats.h"
#include "GLInclude.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>

namespace Engine
{
namespace
{
void BuildLineBuffer(const std::vector<glm::vec3>& points, std::uint32_t& vao, std::uint32_t& vbo, int& count)
{
    count = static_cast<int>(points.size());

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(points.size() * sizeof(glm::vec3)), points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void DrawLines(std::uint32_t vao, int count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, count);
    glBindVertexArray(0);
}
}

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
    ResourceStats::OnMeshDestroyed(m_vertices.size() * sizeof(Vertex) + m_indices.size() * sizeof(std::uint32_t));
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);

    glDeleteBuffers(1, &m_triangleNormalsVbo);
    glDeleteVertexArrays(1, &m_triangleNormalsVao);
    glDeleteBuffers(1, &m_vertexNormalsVbo);
    glDeleteVertexArrays(1, &m_vertexNormalsVao);
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

    ResourceStats::OnMeshCreated(m_vertices.size() * sizeof(Vertex) + m_indices.size() * sizeof(std::uint32_t));
    ENGINE_LOGF("Mesh '{}' uploaded: {} vertices, {} triangles", m_name, m_vertices.size(), GetTriangleCount());
}

void Mesh::Draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::DrawTriangleNormals() const
{
    if (m_triangleNormalsVao == 0)
    {
        const float length = std::max(glm::length(m_aabbMax - m_aabbMin) * 0.05f, 1.0e-4f);

        std::vector<glm::vec3> points;
        points.reserve(m_indices.size() / 3 * 2);
        for (std::size_t i = 0; i + 2 < m_indices.size(); i += 3)
        {
            const glm::vec3& p0 = m_vertices[m_indices[i]].position;
            const glm::vec3& p1 = m_vertices[m_indices[i + 1]].position;
            const glm::vec3& p2 = m_vertices[m_indices[i + 2]].position;

            const glm::vec3 cross = glm::cross(p1 - p0, p2 - p0);
            const float crossLength = glm::length(cross);
            if (crossLength < 1.0e-12f)
            {
                continue;
            }
            const glm::vec3 center = (p0 + p1 + p2) / 3.0f;
            points.push_back(center);
            points.push_back(center + (cross / crossLength) * length);
        }
        BuildLineBuffer(points, m_triangleNormalsVao, m_triangleNormalsVbo, m_triangleNormalsCount);
    }
    DrawLines(m_triangleNormalsVao, m_triangleNormalsCount);
}

void Mesh::DrawVertexNormals() const
{
    if (m_vertexNormalsVao == 0)
    {
        const float length = std::max(glm::length(m_aabbMax - m_aabbMin) * 0.05f, 1.0e-4f);

        std::vector<glm::vec3> points;
        points.reserve(m_vertices.size() * 2);
        for (const Vertex& vertex : m_vertices)
        {
            points.push_back(vertex.position);
            points.push_back(vertex.position + vertex.normal * length);
        }
        BuildLineBuffer(points, m_vertexNormalsVao, m_vertexNormalsVbo, m_vertexNormalsCount);
    }
    DrawLines(m_vertexNormalsVao, m_vertexNormalsCount);
}
}
