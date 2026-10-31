#include "Engine/CameraController.h"

#include <algorithm>
#include <cmath>

namespace Engine
{
namespace
{
constexpr float kMinDistance = 0.01f;
constexpr float kMaxDistance = 1.0e6f;
constexpr float kDollyDragFactor = 0.01f;   // Alt + botó dret
constexpr float kMinSpeed = 0.01f;
constexpr float kMaxSpeed = 10000.0f;
constexpr float kVelocitySmoothing = 12.0f; // com més alt, més sec
constexpr float kFocusSmoothing = 10.0f;
} // namespace

CameraController::CameraController()
{
    Frame(glm::vec3(0.0f), 1.5f, true, false);
}

void CameraController::Frame(const glm::vec3& center, float radius, bool resetOrientation, bool animate)
{
    radius = std::max(radius, 0.001f);
    if (resetOrientation)
    {
        m_camera.LookDirection(-glm::vec3(0.6f, 0.45f, 0.8f));
    }

    const float distance = std::clamp(radius / std::sin(glm::radians(m_camera.GetFovDegrees()) * 0.5f) * 1.2f, kMinDistance, kMaxDistance);
    const glm::vec3 targetPosition = center - m_camera.Forward() * distance;

    // La velocitat de vol s'adapta a la mida del que s'ha enquadrat.
    m_moveSpeed = std::clamp(distance * 0.5f, 0.5f, kMaxSpeed);

    if (animate)
    {
        m_focusing = true;
        m_focusPosition = targetPosition;
        m_focusDistance = distance;
    }
    else
    {
        m_focusing = false;
        m_camera.SetPosition(targetPosition);
        m_distance = distance;
    }
}

void CameraController::Update(const CameraInput& input, float deltaTime)
{
    deltaTime = std::clamp(deltaTime, 0.0f, 0.1f);
    const bool hasWheel = input.wheel != 0.0f;

    // Qualsevol acció de l'usuari cancel·la l'animació de focus.
    if (input.flyActive || input.orbitActive || input.panActive || input.dollyActive || hasWheel)
    {
        m_focusing = false;
    }

    if (input.flyActive)
    {
        Fly(input, deltaTime);
        if (hasWheel)
        {
            // Com a Unity: la roda mentre vols canvia la velocitat de moviment.
            m_moveSpeed = std::clamp(m_moveSpeed * std::pow(m_settings.speedWheelFactor, input.wheel), kMinSpeed, kMaxSpeed);
        }
    }
    else
    {
        m_velocity = glm::vec3(0.0f);
        if (hasWheel)
        {
            ZoomBy(std::exp(-input.wheel * m_settings.wheelZoomFactor));
        }
    }

    if (input.orbitActive)
    {
        Orbit(input);
    }
    if (input.panActive)
    {
        Pan(input);
    }
    if (input.dollyActive)
    {
        ZoomBy(std::exp(-(input.mouseDelta.x - input.mouseDelta.y) * kDollyDragFactor));
    }

    if (m_focusing)
    {
        UpdateFocusAnimation(deltaTime);
    }
    UpdateClipPlanes();
}

void CameraController::Fly(const CameraInput& input, float deltaTime)
{
    // Free look: el pivot "viatja" amb la càmera perquè es calcula a partir de la posició.
    m_camera.SetYawPitch(m_camera.GetYaw() + input.mouseDelta.x * m_settings.lookSensitivity,
                         m_camera.GetPitch() - input.mouseDelta.y * m_settings.lookSensitivity);

    glm::vec3 direction(0.0f);
    if (input.moveForward)
    {
        direction += m_camera.Forward();
    }
    if (input.moveBack)
    {
        direction -= m_camera.Forward();
    }
    if (input.moveRight)
    {
        direction += m_camera.Right();
    }
    if (input.moveLeft)
    {
        direction -= m_camera.Right();
    }
    if (input.moveUp)
    {
        direction += glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (input.moveDown)
    {
        direction -= glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 targetVelocity(0.0f);
    if (glm::dot(direction, direction) > 0.0f)
    {
        targetVelocity = glm::normalize(direction) * m_moveSpeed * (input.fast ? 2.0f : 1.0f);
    }

    // Acceleració suau, independent de la velocitat de refresc.
    const float blend = 1.0f - std::exp(-kVelocitySmoothing * deltaTime);
    m_velocity = glm::mix(m_velocity, targetVelocity, blend);
    m_camera.SetPosition(m_camera.GetPosition() + m_velocity * deltaTime);
}

void CameraController::Orbit(const CameraInput& input)
{
    const glm::vec3 pivot = Pivot();
    m_camera.SetYawPitch(m_camera.GetYaw() + input.mouseDelta.x * m_settings.lookSensitivity,
                         m_camera.GetPitch() - input.mouseDelta.y * m_settings.lookSensitivity);
    m_camera.SetPosition(pivot - m_camera.Forward() * m_distance);
}

void CameraController::Pan(const CameraInput& input)
{
    // Píxels -> món a la distància del pivot, perquè l'escena "segueixi" el cursor.
    const float worldPerPixel = 2.0f * m_distance * std::tan(glm::radians(m_camera.GetFovDegrees()) * 0.5f) / std::max(input.viewportHeight, 1.0f);
    const glm::vec3 offset = (-m_camera.Right() * input.mouseDelta.x + m_camera.Up() * input.mouseDelta.y) * worldPerPixel;
    m_camera.SetPosition(m_camera.GetPosition() + offset);
}

void CameraController::ZoomBy(float factor)
{
    const glm::vec3 pivot = Pivot();
    m_distance = std::clamp(m_distance * factor, kMinDistance, kMaxDistance);
    m_camera.SetPosition(pivot - m_camera.Forward() * m_distance);
}

void CameraController::UpdateFocusAnimation(float deltaTime)
{
    const float blend = 1.0f - std::exp(-kFocusSmoothing * deltaTime);
    m_camera.SetPosition(glm::mix(m_camera.GetPosition(), m_focusPosition, blend));
    m_distance = glm::mix(m_distance, m_focusDistance, blend);

    const float tolerance = 1.0e-3f * m_focusDistance;
    if (glm::distance(m_camera.GetPosition(), m_focusPosition) < tolerance && std::abs(m_distance - m_focusDistance) < tolerance)
    {
        m_camera.SetPosition(m_focusPosition);
        m_distance = m_focusDistance;
        m_focusing = false;
    }
}

void CameraController::UpdateClipPlanes()
{
    // Plans proporcionals a la distància al pivot: serveixen per a models petits i per a models enormes.
    m_camera.SetClipPlanes(std::max(0.01f, m_distance * 0.002f), std::max(1000.0f, m_distance * 200.0f));
}
} // namespace Engine
