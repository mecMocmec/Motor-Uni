#pragma once

#include "Engine/CameraController.h"
#include "Engine/Renderer.h"
#include "Engine/Scene.h"

#include <SDL3/SDL.h>

#include <cstdint>
#include <functional>
#include <string>

namespace Engine
{
class Application
{
public:
    bool Init(const char* title, int width, int height);
    void Shutdown();

    // Processa els events de SDL. 'onEvent' deixa que l'editor també els rebi (ImGui).
    void PumpEvents(const std::function<void(const SDL_Event&)>& onEvent);

    // Importa un model (FBX) i l'afegeix a l'escena. La ruta és UTF-8.
    bool LoadModel(const std::string& utf8Path);

    void Update();
    void ClearBackbuffer();
    void Present();

    bool IsRunning() const { return m_running; }
    void RequestQuit() { m_running = false; }

    float TimeSeconds() const;
    float DeltaTime() const { return m_deltaTime; } // segons del darrer frame

    SDL_Window* GetWindow() const { return m_window; }
    SDL_GLContext GetGLContext() const { return m_glContext; }
    Renderer& GetRenderer() { return m_renderer; }
    Scene& GetScene() { return m_scene; }
    CameraController& GetEditorCamera() { return m_editorCamera; }

private:
    void LoadDefaultModel();
    void HandleDroppedFile(const std::string& utf8Path);

    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    Renderer m_renderer;
    Scene m_scene;
    CameraController m_editorCamera;
    std::uint64_t m_lastTicksNs = 0;
    float m_deltaTime = 0.016f;
    bool m_running = false;
};
} // namespace Engine
