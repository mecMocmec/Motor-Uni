#pragma once

#include "Engine/Component.h"
#include "Engine/Mesh.h"

#include <memory>
#include <utility>

namespace Engine
{
class ComponentMesh : public Component
{
public:
    static constexpr ComponentType kType = ComponentType::Mesh;

    ComponentMesh(GameObject& owner, std::shared_ptr<Mesh> mesh) : Component(kType, owner), m_mesh(std::move(mesh)) {}

    const std::shared_ptr<Mesh>& GetMesh() const { return m_mesh; }

    bool ShowTriangleNormals() const { return m_showTriangleNormals; }
    void SetShowTriangleNormals(bool show) { m_showTriangleNormals = show; }
    bool ShowVertexNormals() const { return m_showVertexNormals; }
    void SetShowVertexNormals(bool show) { m_showVertexNormals = show; }

private:
    std::shared_ptr<Mesh> m_mesh;
    bool m_showTriangleNormals = false;
    bool m_showVertexNormals = false;
};
}
