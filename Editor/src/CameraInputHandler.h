#pragma once

#include <Engine/CameraController.h>

#include <SDL3/SDL.h>

namespace Editor {
    class CameraInputHandler
{
public:
    
    bool ProcessEvent(const SDL_Event& event, SDL_Window* window, bool textInputActive);
    
    void SetSceneState(bool hovered, float viewportHeight);

    Engine::CameraInput BuildInput();
    bool ConsumeFocusRequest();

    bool IsCapturing() const { return m_captureMask != 0; }
    void ReleaseCapture(SDL_Window* window);

private:
    static constexpr unsigned kLeft = 1;
    static constexpr unsigned kMiddle = 2;
    static constexpr unsigned kRight = 4;

    unsigned m_captureMask = 0; 
    bool m_sceneHovered = false;
    float m_viewportHeight = 1.0f;

    float m_deltaX = 0.0f;
    float m_deltaY = 0.0f;
    float m_wheel = 0.0f;
    bool m_focusRequested = false;
};
} 
