#pragma once

#include <Engine/CameraController.h>

#include <SDL3/SDL.h>

namespace Editor
{
// Tradueix els events de SDL als controls de càmera d'Unity:
//   botó dret + WASD/QE (Shift x2, roda = velocitat) | roda = zoom | Alt + esquerre = orbitar
//   botó del mig = desplaçar | Alt + botó dret = zoom arrossegant | F = enfocar
class CameraInputHandler
{
public:
    // Retorna true si l'event s'ha de retenir (no passar-lo a ImGui): moviment i roda mentre es captura el ratolí.
    bool ProcessEvent(const SDL_Event& event, SDL_Window* window, bool textInputActive);

    // L'editor diu si el ratolí és sobre la finestra Escena i l'alçada de la vista (per al desplaçament).
    void SetSceneState(bool hovered, float viewportHeight);

    // Construeix l'entrada d'aquest frame i reinicia els acumuladors.
    Engine::CameraInput BuildInput();
    bool ConsumeFocusRequest();

    bool IsCapturing() const { return m_captureMask != 0; }
    void ReleaseCapture(SDL_Window* window);

private:
    static constexpr unsigned kLeft = 1;
    static constexpr unsigned kMiddle = 2;
    static constexpr unsigned kRight = 4;

    unsigned m_captureMask = 0; // botons que han començat una captura dins de l'Escena
    bool m_sceneHovered = false;
    float m_viewportHeight = 1.0f;

    float m_deltaX = 0.0f;
    float m_deltaY = 0.0f;
    float m_wheel = 0.0f;
    bool m_focusRequested = false;
};
} // namespace Editor
