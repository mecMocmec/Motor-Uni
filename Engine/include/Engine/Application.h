#pragma once

#include "Engine/CameraController.h"
#include "Engine/Renderer.h"
#include "Engine/SystemInfo.h"
#include "Engine/Scene.h"

#include <SDL3/SDL.h>

#include <array>
#include <cstdint>
#include <functional>
#include <string>

namespace Engine
{
struct FrameHistory
{
    static constexpr int kSamples = 120;
    std::array<float, kSamples> fps{};
    std::array<float, kSamples> frameMs{};
    int offset = 0;         
    float currentFps = 0.0f; 
};

class Application
{
public:
    bool Init(const char* title, int width, int height);
    void Shutdown();

    void PumpEvents(const std::function<void(const SDL_Event&)>& onEvent);

    bool LoadModel(const std::string& utf8Path);

    void Update();
    void ClearBackbuffer();
    void Present();

    bool IsRunning() const { return m_running; }
    void RequestQuit() { m_running = false; }

    float TimeSeconds() const;
    float DeltaTime() const { return m_deltaTime; } 

    void GetWindowSize(int& width, int& height) const; 
    void SetWindowSize(int width, int height);
    bool IsFullscreen() const;
    void SetFullscreen(bool fullscreen);
    bool IsResizable() const;
    void SetResizable(bool resizable);
    bool IsBordered() const;
    void SetBordered(bool bordered);
    bool IsVSync() const { return m_vsync; }
    void SetVSync(bool vsync);
    int GetFpsLimit() const { return m_fpsLimit; } 
    void SetFpsLimit(int fps) { m_fpsLimit = fps; }

    const FrameHistory& GetFrameHistory() const { return m_frameHistory; }
    const SystemInfo& GetSystemInfo() const { return m_systemInfo; }
    MemoryInfo QueryMemory() const;

    SDL_Window* GetWindow() const { return m_window; }
    SDL_GLContext GetGLContext() const { return m_glContext; }
    Renderer& GetRenderer() { return m_renderer; }
    Scene& GetScene() { return m_scene; }
    CameraController& GetEditorCamera() { return m_editorCamera; }

private:
    void LoadDefaultModel();
    void RecordFrame(float deltaTime);
    void LimitFrameRate();
    void HandleDroppedFile(const std::string& utf8Path);

    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    Renderer m_renderer;
    Scene m_scene;
    CameraController m_editorCamera;
    std::uint64_t m_lastTicksNs = 0;
    float m_deltaTime = 0.016f;
    FrameHistory m_frameHistory;
    SystemInfo m_systemInfo;
    bool m_vsync = true;
    int m_fpsLimit = 0;
    bool m_running = false;
};
} 
