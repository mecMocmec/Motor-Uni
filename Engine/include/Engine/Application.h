#pragma once

#include "Engine/Renderer.h"
#include "Engine/Scene.h"

#include <SDL3/SDL.h>

#include <functional>

namespace Engine
{
class Application
{
public:
    bool Init(const char* title, int width, int height);
    void Shutdown();

    // Processa els events de SDL. 'onEvent' deixa que l'editor també els rebi (ImGui).
    void PumpEvents(const std::function<void(const SDL_Event&)>& onEvent);

    void Update();
    void ClearBackbuffer();
    void Present();

    bool IsRunning() const { return m_running; }
    void RequestQuit() { m_running = false; }

    float TimeSeconds() const;

    SDL_Window* GetWindow() const { return m_window; }
    SDL_GLContext GetGLContext() const { return m_glContext; }
    Renderer& GetRenderer() { return m_renderer; }
    Scene& GetScene() { return m_scene; }

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    Renderer m_renderer;
    Scene m_scene;
    bool m_running = false;
};
} // namespace Engine
