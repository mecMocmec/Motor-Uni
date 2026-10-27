#pragma once

#include "Engine/Component.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Engine
{
class ComponentTransform;

// Un GameObject conté 1 Transform (sempre) + 0..N components i una llista de fills.
class GameObject
{
public:
    explicit GameObject(std::string name, GameObject* parent = nullptr);
    ~GameObject();

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    const std::string& GetName() const { return m_name; }
    void SetName(std::string name) { m_name = std::move(name); }
    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    GameObject* GetParent() const { return m_parent; }
    const std::vector<std::unique_ptr<GameObject>>& GetChildren() const { return m_children; }

    GameObject& CreateChild(std::string name);
    void RemoveChild(const GameObject* child);
    bool IsDescendantOf(const GameObject* ancestor) const;

    ComponentTransform& GetTransform() const { return *m_transform; }

    template <class T, class... Args>
    T& AddComponent(Args&&... args)
    {
        auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& reference = *component;
        m_components.push_back(std::move(component));
        return reference;
    }

    // Retorna el primer component d'aquest tipus, o nullptr si no n'hi ha.
    template <class T>
    T* GetComponent() const
    {
        for (const auto& component : m_components)
        {
            if (component->GetType() == T::kType)
            {
                return static_cast<T*>(component.get());
            }
        }
        return nullptr;
    }

    // Recalcula les matrius globals de tot el subarbre.
    void UpdateTransforms(const glm::mat4& parentGlobal);

private:
    std::string m_name;
    bool m_active = true;
    GameObject* m_parent = nullptr;
    std::vector<std::unique_ptr<GameObject>> m_children;
    std::vector<std::unique_ptr<Component>> m_components;
    ComponentTransform* m_transform = nullptr;
};
} // namespace Engine
