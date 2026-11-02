#include "Engine/GameObject.h"

#include "Engine/ComponentMesh.h"
#include "Engine/ComponentTransform.h"

#include <algorithm>
#include <limits>

namespace Engine
{
namespace
{
void AccumulateAabb(const GameObject& gameObject, glm::vec3& outMin, glm::vec3& outMax, bool& found)
{
    const ComponentMesh* meshComponent = gameObject.GetComponent<ComponentMesh>();
    if (meshComponent != nullptr && meshComponent->GetMesh() != nullptr)
    {
        const Mesh& mesh = *meshComponent->GetMesh();
        const glm::mat4& global = gameObject.GetTransform().GetGlobalMatrix();
        for (int corner = 0; corner < 8; ++corner)
        {
            const glm::vec3 local((corner & 1) ? mesh.GetAabbMax().x : mesh.GetAabbMin().x,
                                  (corner & 2) ? mesh.GetAabbMax().y : mesh.GetAabbMin().y,
                                  (corner & 4) ? mesh.GetAabbMax().z : mesh.GetAabbMin().z);
            const glm::vec3 world = glm::vec3(global * glm::vec4(local, 1.0f));
            outMin = glm::min(outMin, world);
            outMax = glm::max(outMax, world);
            found = true;
        }
    }
    for (const auto& child : gameObject.GetChildren())
    {
        AccumulateAabb(*child, outMin, outMax, found);
    }
}
} 

GameObject::GameObject(std::string name, GameObject* parent) : m_name(std::move(name)), m_parent(parent)
{
    m_transform = &AddComponent<ComponentTransform>();
}

GameObject::~GameObject() = default;

GameObject& GameObject::CreateChild(std::string name)
{
    m_children.push_back(std::make_unique<GameObject>(std::move(name), this));
    return *m_children.back();
}

void GameObject::RemoveChild(const GameObject* child)
{
    std::erase_if(m_children, [child](const std::unique_ptr<GameObject>& candidate) { return candidate.get() == child; });
}

bool GameObject::IsDescendantOf(const GameObject* ancestor) const
{
    for (const GameObject* current = m_parent; current != nullptr; current = current->m_parent)
    {
        if (current == ancestor)
        {
            return true;
        }
    }
    return false;
}

bool GameObject::ComputeWorldAabb(glm::vec3& outMin, glm::vec3& outMax) const
{
    outMin = glm::vec3(std::numeric_limits<float>::max());
    outMax = glm::vec3(std::numeric_limits<float>::lowest());
    bool found = false;
    AccumulateAabb(*this, outMin, outMax, found);
    return found;
}

void GameObject::UpdateTransforms(const glm::mat4& parentGlobal)
{
    m_transform->UpdateGlobal(parentGlobal);
    for (const auto& child : m_children)
    {
        child->UpdateTransforms(m_transform->GetGlobalMatrix());
    }
}
} 
