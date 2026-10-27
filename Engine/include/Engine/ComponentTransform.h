#pragma once

#include "Engine/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
// Posició, rotació (quaternió) i escala locals + matriu global.
class ComponentTransform : public Component
{
public:
    static constexpr ComponentType kType = ComponentType::Transform;

    explicit ComponentTransform(GameObject& owner);

    const glm::vec3& GetPosition() const { return m_position; }
    const glm::quat& GetRotation() const { return m_rotation; }
    const glm::vec3& GetScale() const { return m_scale; }
    // Angles d'Euler en graus, només per mostrar/editar a l'inspector.
    const glm::vec3& GetEulerDegrees() const { return m_eulerDegrees; }

    void SetPosition(const glm::vec3& position) { m_position = position; }
    void SetScale(const glm::vec3& scale) { m_scale = scale; }
    void SetRotation(const glm::quat& rotation);
    void SetEulerDegrees(const glm::vec3& eulerDegrees);

    glm::mat4 GetLocalMatrix() const;
    const glm::mat4& GetGlobalMatrix() const { return m_global; }

    // global = pare.global * local. Es crida cada frame des de l'arrel.
    void UpdateGlobal(const glm::mat4& parentGlobal);

private:
    glm::vec3 m_position{0.0f};
    glm::quat m_rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale{1.0f};
    glm::vec3 m_eulerDegrees{0.0f};
    glm::mat4 m_global{1.0f};
};
} // namespace Engine
