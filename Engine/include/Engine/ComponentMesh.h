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

private:
    std::shared_ptr<Mesh> m_mesh;
};
} // namespace Engine
