#pragma once

#include <glm/glm.hpp>

namespace Engine
{
class Camera
{
public:
    const glm::vec3& GetPosition() const { return m_position; }
    void SetPosition(const glm::vec3& position) { m_position = position; }

    float GetYaw() const { return m_yaw; }     
    float GetPitch() const { return m_pitch; } 
    void SetYawPitch(float yaw, float pitch);
    void LookDirection(const glm::vec3& direction);

    glm::vec3 Forward() const;
    glm::vec3 Right() const;
    glm::vec3 Up() const;

    float GetFovDegrees() const { return m_fovDegrees; }
    void SetFovDegrees(float fovDegrees) { m_fovDegrees = fovDegrees; }
    float GetNearPlane() const { return m_near; }
    float GetFarPlane() const { return m_far; }
    void SetClipPlanes(float nearPlane, float farPlane);

    glm::mat4 ViewMatrix() const;
    glm::mat4 ProjectionMatrix(float aspectRatio) const;

private:
    glm::vec3 m_position{0.0f, 0.0f, 5.0f};
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_fovDegrees = 60.0f; 
    float m_near = 0.1f;
    float m_far = 1000.0f;
};
} 
