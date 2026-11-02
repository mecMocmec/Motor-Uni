#pragma once

#include <array>

namespace Engine {
    class Application;
}

namespace Editor
{
class ConfigurationPanel
{
public:
    void Draw(Engine::Application& app, bool& open, bool cameraCapturing);

private:
    void DrawFpsSection(Engine::Application& app);
    void DrawWindowSection(Engine::Application& app);
    void DrawRendererSection(Engine::Application& app);
    void DrawCameraSection(Engine::Application& app);
    void DrawInputSection(Engine::Application& app, bool cameraCapturing);
    void DrawTexturesSection();
    void DrawSceneSection(Engine::Application& app);
    void DrawMemorySection(Engine::Application& app);
    void DrawHardwareSection(Engine::Application& app);
    void DrawSoftwareSection(Engine::Application& app);

    static constexpr int kMemorySamples = 120;
    std::array<float, kMemorySamples> m_memoryHistoryMb{};
    int m_memoryOffset = 0;
    float m_memoryTimer = 0.0f;

    int m_pendingWindowSize[2] = {0, 0};
};
}
