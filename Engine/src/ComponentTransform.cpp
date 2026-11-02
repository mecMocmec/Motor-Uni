#include "Engine/ComponentTransform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
ComponentTransform::ComponentTransform(GameObject& owner) : Component(kType, owner) {}

void ComponentTransform::SetRotation(const glm::quat& rotation)
{
    m_rotation = glm::normalize(rotation);
    m_eulerDegrees = glm::degrees(glm::eulerAngles(m_rotation));
}

void ComponentTransform::SetEulerDegrees(const glm::vec3& eulerDegrees)
{
    m_eulerDegrees = eulerDegrees;
    m_rotation = glm::quat(glm::radians(eulerDegrees));
}

glm::mat4 ComponentTransform::GetLocalMatrix() const
{
    return glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
}

void ComponentTransform::UpdateGlobal(const glm::mat4& parentGlobal)
{
    m_global = parentGlobal * GetLocalMatrix();
}
}
