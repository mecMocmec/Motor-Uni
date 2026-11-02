#pragma once

#include "Engine/Camera.h"

#include <glm/glm.hpp>

namespace Engine
{
struct CameraInput
{
    glm::vec2 mouseDelta{0.0f}; 
    float wheel = 0.0f;         
    float viewportHeight = 1.0f;

    bool flyActive = false;   
    bool orbitActive = false; 
    bool panActive = false;   
    bool dollyActive = false; 

    bool moveForward = false;
    bool moveBack = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;   
    bool moveDown = false; 
    bool fast = false;     
};
.
struct CameraSettings
{
    float lookSensitivity = 0.0035f; 
    float wheelZoomFactor = 0.15f;   
    float speedWheelFactor = 1.2f;   
};

class CameraController
{
public:
    CameraController();

    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }
    float GetMoveSpeed() const { return m_moveSpeed; }
    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
    CameraSettings& GetSettings() { return m_settings; }

    void Frame(const glm::vec3& center, float radius, bool resetOrientation, bool animate);

    void Update(const CameraInput& input, float deltaTime);

private:
    glm::vec3 Pivot() const { return m_camera.GetPosition() + m_camera.Forward() * m_distance; }
    void Fly(const CameraInput& input, float deltaTime);
    void Orbit(const CameraInput& input);
    void Pan(const CameraInput& input);
    void ZoomBy(float factor);
    void UpdateFocusAnimation(float deltaTime);
    void UpdateClipPlanes();

    Camera m_camera;
    CameraSettings m_settings;
    float m_distance = 5.0f;  
    float m_moveSpeed = 3.0f; 
    glm::vec3 m_velocity{0.0f};

    bool m_focusing = false;
    glm::vec3 m_focusPosition{0.0f};
    float m_focusDistance = 5.0f;
};
} 
