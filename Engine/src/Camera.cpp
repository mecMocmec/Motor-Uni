#include "Engine/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

namespace Engine
{
namespace
{
const float kMaxPitch = glm::radians(89.0f);
}

void Camera::SetYawPitch(float yaw, float pitch)
{
    m_yaw = yaw;
    m_pitch = std::clamp(pitch, -kMaxPitch, kMaxPitch);
}

void Camera::LookDirection(const glm::vec3& direction)
{
    const glm::vec3 d = glm::normalize(direction);
    SetYawPitch(std::atan2(d.x, -d.z), std::asin(std::clamp(d.y, -1.0f, 1.0f)));
}

glm::vec3 Camera::Forward() const
{
    return glm::vec3(std::sin(m_yaw) * std::cos(m_pitch), std::sin(m_pitch), -std::cos(m_yaw) * std::cos(m_pitch));
}

glm::vec3 Camera::Right() const
{
    return glm::vec3(std::cos(m_yaw), 0.0f, std::sin(m_yaw));
}

glm::vec3 Camera::Up() const
{
    return glm::cross(Right(), Forward());
}

void Camera::SetClipPlanes(float nearPlane, float farPlane)
{
    m_near = nearPlane;
    m_far = farPlane;
}

glm::mat4 Camera::ViewMatrix() const
{
    return glm::lookAt(m_position, m_position + Forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::ProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(m_fovDegrees), aspectRatio, m_near, m_far);
}
} 
