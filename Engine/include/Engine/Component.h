#pragma once

namespace Engine
{
class GameObject;

enum class ComponentType
{
    Transform,
    Mesh,
    Texture // s'afegirà amb la càrrega de textures (DevIL)
};

// Classe base de tots els components. Cada component pertany a un GameObject.
class Component
{
public:
    Component(ComponentType type, GameObject& owner) : m_type(type), m_owner(&owner) {}
    virtual ~Component() = default;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    ComponentType GetType() const { return m_type; }
    GameObject& GetOwner() const { return *m_owner; }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

private:
    ComponentType m_type;
    GameObject* m_owner;
    bool m_active = true;
};
} // namespace Engine
