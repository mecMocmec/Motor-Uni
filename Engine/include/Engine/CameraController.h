#pragma once

#include "Engine/Camera.h"

#include <glm/glm.hpp>

namespace Engine
{
// Resum de l'entrada d'un frame. L'editor l'omple; així el motor no depèn de SDL ni d'ImGui.
struct CameraInput
{
    glm::vec2 mouseDelta{0.0f}; // píxels moguts aquest frame
    float wheel = 0.0f;         // voltes de roda aquest frame (positiu = endavant)
    float viewportHeight = 1.0f;

    bool flyActive = false;   // botó dret: mirar al voltant + WASD
    bool orbitActive = false; // Alt + botó esquerre
    bool panActive = false;   // botó del mig
    bool dollyActive = false; // Alt + botó dret

    bool moveForward = false;
    bool moveBack = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;   // E
    bool moveDown = false; // Q
    bool fast = false;     // Shift: velocitat x2
};

// Mòdul de la càmera de l'editor, amb els controls de Unity.
// Manté un "pivot" implícit a 'distance' davant de la càmera: és el punt al voltant del qual s'orbita.
class CameraController
{
public:
    CameraController();

    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }
    float GetMoveSpeed() const { return m_moveSpeed; }

    // Enquadra una esfera (centre, radi). 'resetOrientation' mira des d'una diagonal; si no, es manté l'orientació.
    // Amb 'animate' la càmera hi va suaument (com la tecla F d'Unity).
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
    float m_distance = 5.0f;  // distància al pivot
    float m_moveSpeed = 3.0f; // unitats per segon en mode volar
    glm::vec3 m_velocity{0.0f};

    bool m_focusing = false;
    glm::vec3 m_focusPosition{0.0f};
    float m_focusDistance = 5.0f;
};
} // namespace Engine
