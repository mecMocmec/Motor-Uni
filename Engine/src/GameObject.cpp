#include "Engine/GameObject.h"

#include "Engine/ComponentTransform.h"

#include <algorithm>

namespace Engine
{
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

void GameObject::UpdateTransforms(const glm::mat4& parentGlobal)
{
    m_transform->UpdateGlobal(parentGlobal);
    for (const auto& child : m_children)
    {
        child->UpdateTransforms(m_transform->GetGlobalMatrix());
    }
}
} // namespace Engine
