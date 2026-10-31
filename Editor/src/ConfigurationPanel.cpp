#include "ConfigurationPanel.h"

#include <Engine/Application.h>
#include <Engine/CameraController.h>
#include <Engine/Renderer.h>
#include <Engine/ResourceStats.h>
#include <Engine/Scene.h>
#include <Engine/Texture.h>

#include <SDL3/SDL.h>
#include <imgui.h>

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>

namespace Editor
{
namespace
{
float ToMb(std::uint64_t bytes)
{
    return static_cast<float>(static_cast<double>(bytes) / (1024.0 * 1024.0));
}

float KbToMb(std::uint64_t kilobytes)
{
    return static_cast<float>(static_cast<double>(kilobytes) / 1024.0);
}
} // namespace

void ConfigurationPanel::Draw(Engine::Application& app, bool& open, bool cameraCapturing)
{
    ImGui::SetNextWindowSize(ImVec2(460.0f, 680.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Configuracion", &open))
    {
        DrawFpsSection(app);
        DrawWindowSection(app);
        DrawRendererSection(app);
        DrawCameraSection(app);
        DrawInputSection(app, cameraCapturing);
        DrawTexturesSection();
        DrawSceneSection(app);
        DrawMemorySection(app);
        DrawHardwareSection(app);
        DrawSoftwareSection(app);
    }
    ImGui::End();
}

void ConfigurationPanel::DrawFpsSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("FPS", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    const Engine::FrameHistory& history = app.GetFrameHistory();

    float sumFps = 0.0f;
    float minFps = FLT_MAX;
    float maxFps = 0.0f;
    float maxMs = 0.0f;
    int count = 0;
    for (int i = 0; i < Engine::FrameHistory::kSamples; ++i)
    {
        const float fps = history.fps[static_cast<std::size_t>(i)];
        if (fps <= 0.0f)
        {
            continue;
        }
        sumFps += fps;
        minFps = std::min(minFps, fps);
        maxFps = std::max(maxFps, fps);
        maxMs = std::max(maxMs, history.frameMs[static_cast<std::size_t>(i)]);
        ++count;
    }
    const float averageFps = count > 0 ? sumFps / static_cast<float>(count) : 0.0f;
    if (count == 0)
    {
        minFps = 0.0f;
    }

    ImGui::Text("FPS: %.1f   (%.2f ms/frame)", history.currentFps, history.currentFps > 0.0f ? 1000.0f / history.currentFps : 0.0f);
    ImGui::Text("Average %.1f | Min %.1f | Max %.1f", averageFps, minFps, maxFps);

    char overlay[32];
    std::snprintf(overlay, sizeof(overlay), "%.1f FPS", history.currentFps);
    ImGui::PlotLines("##fps", history.fps.data(), Engine::FrameHistory::kSamples, history.offset, overlay, 0.0f, std::max(60.0f, maxFps * 1.2f),
                     ImVec2(0.0f, 80.0f));

    std::snprintf(overlay, sizeof(overlay), "%.2f ms", history.currentFps > 0.0f ? 1000.0f / history.currentFps : 0.0f);
    ImGui::PlotLines("##frametime", history.frameMs.data(), Engine::FrameHistory::kSamples, history.offset, overlay, 0.0f, std::max(33.3f, maxMs * 1.2f),
                     ImVec2(0.0f, 60.0f));

    int limit = app.GetFpsLimit();
    if (ImGui::SliderInt("FPS limit (0 = no limit)", &limit, 0, 240))
    {
        app.SetFpsLimit(limit);
    }
    if (app.IsVSync() && limit > 0)
    {
        ImGui::TextDisabled("VSync is on: the limit only matters below the display refresh rate.");
    }
}

void ConfigurationPanel::DrawWindowSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Window"))
    {
        return;
    }

    int width = 0;
    int height = 0;
    app.GetWindowSize(width, height);
    ImGui::Text("Current size: %d x %d px", width, height);

    if (m_pendingWindowSize[0] == 0)
    {
        m_pendingWindowSize[0] = width;
        m_pendingWindowSize[1] = height;
    }
    ImGui::InputInt2("New size", m_pendingWindowSize);
    ImGui::SameLine();
    if (ImGui::Button("Apply"))
    {
        app.SetWindowSize(m_pendingWindowSize[0], m_pendingWindowSize[1]);
    }

    bool fullscreen = app.IsFullscreen();
    if (ImGui::Checkbox("Fullscreen", &fullscreen))
    {
        app.SetFullscreen(fullscreen);
    }
    bool resizable = app.IsResizable();
    if (ImGui::Checkbox("Resizable", &resizable))
    {
        app.SetResizable(resizable);
    }
    bool bordered = app.IsBordered();
    if (ImGui::Checkbox("Bordered", &bordered))
    {
        app.SetBordered(bordered);
    }
    bool vsync = app.IsVSync();
    if (ImGui::Checkbox("VSync", &vsync))
    {
        app.SetVSync(vsync);
    }

    const Engine::SystemInfo& info = app.GetSystemInfo();
    ImGui::Text("Display: %s  %d x %d @ %.0f Hz", info.displayName.c_str(), info.displayWidth, info.displayHeight, info.displayRefreshRate);
}

void ConfigurationPanel::DrawRendererSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Renderer"))
    {
        return;
    }

    Engine::Renderer& renderer = app.GetRenderer();
    Engine::RendererSettings& settings = renderer.GetSettings();

    ImGui::Checkbox("Depth test (GL_DEPTH_TEST)", &settings.depthTest);
    ImGui::Checkbox("Cull face (GL_CULL_FACE)", &settings.cullFace);
    if (settings.cullFace)
    {
        ImGui::Indent();
        int culled = settings.cullBackFaces ? 0 : 1;
        const char* culledItems[] = {"Back faces", "Front faces"};
        if (ImGui::Combo("Culled faces", &culled, culledItems, 2))
        {
            settings.cullBackFaces = culled == 0;
        }
        int winding = settings.frontFaceCounterClockwise ? 0 : 1;
        const char* windingItems[] = {"Counter-clockwise (CCW)", "Clockwise (CW)"};
        if (ImGui::Combo("Front face", &winding, windingItems, 2))
        {
            settings.frontFaceCounterClockwise = winding == 0;
        }
        ImGui::Unindent();
    }
    ImGui::Checkbox("Wireframe", &settings.wireframe);
    ImGui::Checkbox("Alpha blend (GL_BLEND)", &settings.blend);

    ImGui::BeginDisabled();
    bool lighting = false;
    ImGui::Checkbox("Lighting (GL_LIGHTING)", &lighting);
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::TextDisabled("(not in the core profile)");

    ImGui::ColorEdit3("Clear color", &settings.clearColor.x);
    ImGui::Text("Scene target: %d x %d px", renderer.TargetWidth(), renderer.TargetHeight());

    if (ImGui::Button("Reset renderer"))
    {
        settings = Engine::RendererSettings{};
    }
}

void ConfigurationPanel::DrawCameraSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Camera"))
    {
        return;
    }

    Engine::CameraController& controller = app.GetEditorCamera();
    Engine::Camera& camera = controller.GetCamera();

    float fov = camera.GetFovDegrees();
    if (ImGui::SliderFloat("Vertical FOV", &fov, 30.0f, 120.0f, "%.0f deg"))
    {
        camera.SetFovDegrees(fov);
    }
    // El FOV vertical és el que fixem; l'horitzontal depèn de la relació d'aspecte de la vista.
    const Engine::Renderer& renderer = app.GetRenderer();
    const float aspect = renderer.TargetHeight() > 0 ? static_cast<float>(renderer.TargetWidth()) / static_cast<float>(renderer.TargetHeight()) : 1.0f;
    const float horizontalFov = glm::degrees(2.0f * std::atan(std::tan(glm::radians(fov) * 0.5f) * aspect));
    ImGui::Text("Horizontal FOV: %.1f deg (aspect %.2f)", horizontalFov, aspect);

    const glm::vec3& position = camera.GetPosition();
    ImGui::Text("Position: %.2f, %.2f, %.2f", position.x, position.y, position.z);
    ImGui::Text("Yaw: %.1f deg   Pitch: %.1f deg", glm::degrees(camera.GetYaw()), glm::degrees(camera.GetPitch()));
    ImGui::Text("Near: %.3f   Far: %.1f (automatic)", camera.GetNearPlane(), camera.GetFarPlane());

    float speed = controller.GetMoveSpeed();
    if (ImGui::SliderFloat("Move speed", &speed, 0.01f, 1000.0f, "%.2f", ImGuiSliderFlags_Logarithmic))
    {
        controller.SetMoveSpeed(speed);
    }

    if (ImGui::Button("Reset camera"))
    {
        controller.Frame(glm::vec3(0.0f), 1.5f, true, true);
    }
}

void ConfigurationPanel::DrawInputSection(Engine::Application& app, bool cameraCapturing)
{
    if (!ImGui::CollapsingHeader("Input"))
    {
        return;
    }

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);
    ImGui::Text("Mouse: %.0f, %.0f", mouseX, mouseY);
    ImGui::Text("Buttons: %s %s %s", (buttons & SDL_BUTTON_LMASK) ? "[Left]" : "Left", (buttons & SDL_BUTTON_MMASK) ? "[Middle]" : "Middle",
                (buttons & SDL_BUTTON_RMASK) ? "[Right]" : "Right");

    int keyCount = 0;
    const bool* keys = SDL_GetKeyboardState(&keyCount);
    std::string pressed;
    int shown = 0;
    for (int i = 0; i < keyCount && shown < 8; ++i)
    {
        if (!keys[i])
        {
            continue;
        }
        const char* name = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
        if (name != nullptr && name[0] != '\0')
        {
            pressed += shown > 0 ? ", " : "";
            pressed += name;
            ++shown;
        }
    }
    ImGui::Text("Keys held: %s", pressed.empty() ? "-" : pressed.c_str());
    ImGui::Text("Camera mouse capture: %s", cameraCapturing ? "active" : "inactive");

    ImGui::Separator();
    Engine::CameraSettings& settings = app.GetEditorCamera().GetSettings();

    float sensitivityDegrees = glm::degrees(settings.lookSensitivity);
    if (ImGui::SliderFloat("Mouse sensitivity", &sensitivityDegrees, 0.02f, 0.6f, "%.3f deg/px"))
    {
        settings.lookSensitivity = glm::radians(sensitivityDegrees);
    }
    ImGui::SliderFloat("Wheel zoom", &settings.wheelZoomFactor, 0.02f, 0.5f, "%.2f");
    ImGui::SliderFloat("Wheel speed change", &settings.speedWheelFactor, 1.05f, 2.0f, "x%.2f");

    if (ImGui::Button("Reset input"))
    {
        settings = Engine::CameraSettings{};
    }
}

void ConfigurationPanel::DrawTexturesSection()
{
    if (!ImGui::CollapsingHeader("Textures"))
    {
        return;
    }

    Engine::TextureSettings& settings = Engine::Texture::GetSettings();
    bool changed = false;

    int wrap = static_cast<int>(settings.wrap);
    const char* wrapItems[] = {"Repeat", "Mirrored repeat", "Clamp to edge", "Clamp to border"};
    if (ImGui::Combo("Wrap (S/T)", &wrap, wrapItems, 4))
    {
        settings.wrap = static_cast<Engine::TextureWrap>(wrap);
        changed = true;
    }

    int minFilter = static_cast<int>(settings.minFilter);
    const char* minItems[] = {"Nearest",
                              "Linear",
                              "Nearest mipmap nearest",
                              "Linear mipmap nearest",
                              "Nearest mipmap linear",
                              "Linear mipmap linear"};
    if (ImGui::Combo("Minification filter", &minFilter, minItems, 6))
    {
        settings.minFilter = static_cast<Engine::TextureMinFilter>(minFilter);
        changed = true;
    }

    int magFilter = static_cast<int>(settings.magFilter);
    const char* magItems[] = {"Nearest", "Linear"};
    if (ImGui::Combo("Magnification filter", &magFilter, magItems, 2))
    {
        settings.magFilter = static_cast<Engine::TextureMagFilter>(magFilter);
        changed = true;
    }

    const float maxAnisotropy = Engine::Texture::MaxAnisotropy();
    if (maxAnisotropy > 1.0f)
    {
        if (ImGui::SliderFloat("Anisotropic filtering", &settings.anisotropy, 1.0f, maxAnisotropy, "%.0fx"))
        {
            changed = true;
        }
    }
    else
    {
        ImGui::TextDisabled("Anisotropic filtering not supported by this GPU");
    }
    ImGui::TextDisabled("Mipmaps are generated when a texture is loaded.");

    if (changed)
    {
        Engine::Texture::ApplySettings();
    }

    ImGui::Separator();
    const std::vector<const Engine::Texture*>& textures = Engine::Texture::GetAll();
    ImGui::Text("Loaded textures: %zu", textures.size());
    if (!textures.empty() && ImGui::BeginTable("textures", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("Id");
        ImGui::TableSetupColumn("Size");
        ImGui::TableSetupColumn("MB");
        ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthStretch, 3.0f);
        ImGui::TableHeadersRow();
        for (const Engine::Texture* texture : textures)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%u", texture->GetId());
            ImGui::TableNextColumn();
            ImGui::Text("%dx%d", texture->GetWidth(), texture->GetHeight());
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", ToMb(texture->GetGpuBytes()));
            ImGui::TableNextColumn();
            ImGui::TextWrapped("%s", texture->GetPath().c_str());
        }
        ImGui::EndTable();
    }
}

void ConfigurationPanel::DrawSceneSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Scene resources"))
    {
        return;
    }

    const Engine::SceneStats scene = app.GetScene().ComputeStats();
    ImGui::Text("GameObjects: %d", scene.gameObjects);
    ImGui::Text("Mesh components: %d", scene.meshComponents);
    ImGui::Text("Vertices: %llu   Triangles: %llu", static_cast<unsigned long long>(scene.vertices), static_cast<unsigned long long>(scene.triangles));

    const Engine::ResourceStats::Snapshot gpu = Engine::ResourceStats::Get();
    ImGui::Separator();
    ImGui::Text("GPU meshes: %d (%.2f MB)", gpu.meshCount, ToMb(gpu.meshBytes));
    ImGui::Text("GPU textures: %d (%.2f MB)", gpu.textureCount, ToMb(gpu.textureBytes));
}

void ConfigurationPanel::DrawMemorySection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Memory", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    const Engine::MemoryInfo memory = app.QueryMemory();

    ImGui::Text("Process (working set): %.1f MB   peak %.1f MB", ToMb(memory.processWorkingSetBytes), ToMb(memory.processPeakWorkingSetBytes));
    ImGui::Text("Process (committed): %.1f MB", ToMb(memory.processCommitBytes));

    if (memory.systemTotalBytes > 0)
    {
        const std::uint64_t used = memory.systemTotalBytes - std::min(memory.systemAvailableBytes, memory.systemTotalBytes);
        char overlay[64];
        std::snprintf(overlay, sizeof(overlay), "%.1f / %.1f GB", ToMb(used) / 1024.0f, ToMb(memory.systemTotalBytes) / 1024.0f);
        ImGui::ProgressBar(static_cast<float>(static_cast<double>(used) / static_cast<double>(memory.systemTotalBytes)), ImVec2(-FLT_MIN, 0.0f), overlay);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::TextUnformatted("System RAM");
    }

    if (memory.gpuAvailable)
    {
        if (memory.gpuTotalKb > 0)
        {
            ImGui::Text("GPU memory: %.0f MB free of %.0f MB", KbToMb(memory.gpuFreeKb), KbToMb(memory.gpuTotalKb));
        }
        else
        {
            ImGui::Text("GPU memory free: %.0f MB", KbToMb(memory.gpuFreeKb));
        }
    }
    else
    {
        ImGui::TextDisabled("GPU memory: not reported by this driver");
    }

    // Historial del consum del procés, una mostra cada 0,25 s.
    m_memoryTimer += ImGui::GetIO().DeltaTime;
    if (m_memoryTimer >= 0.25f)
    {
        m_memoryTimer = 0.0f;
        m_memoryHistoryMb[static_cast<std::size_t>(m_memoryOffset)] = ToMb(memory.processWorkingSetBytes);
        m_memoryOffset = (m_memoryOffset + 1) % kMemorySamples;
    }
    float maxMb = 1.0f;
    for (const float value : m_memoryHistoryMb)
    {
        maxMb = std::max(maxMb, value);
    }
    char overlay[32];
    std::snprintf(overlay, sizeof(overlay), "%.1f MB", ToMb(memory.processWorkingSetBytes));
    ImGui::PlotLines("##memory", m_memoryHistoryMb.data(), kMemorySamples, m_memoryOffset, overlay, 0.0f, maxMb * 1.2f, ImVec2(0.0f, 60.0f));
}

void ConfigurationPanel::DrawHardwareSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Hardware"))
    {
        return;
    }

    const Engine::SystemInfo& info = app.GetSystemInfo();
    ImGui::Text("CPU: %s", info.cpuName.c_str());
    ImGui::Text("Logical cores: %d   Cache line: %d B", info.cpuLogicalCores, info.cpuCacheLineBytes);
    ImGui::TextWrapped("Features: %s", info.cpuFeatures.c_str());
    ImGui::Text("System RAM: %d MB", info.systemRamMb);

    ImGui::Separator();
    ImGui::Text("GPU vendor: %s", info.gpuVendor.c_str());
    ImGui::Text("GPU renderer: %s", info.gpuRenderer.c_str());
    ImGui::Text("Max texture size: %d", info.glMaxTextureSize);
    ImGui::Text("Texture units: %d   Vertex attributes: %d", info.glMaxTextureUnits, info.glMaxVertexAttribs);
    ImGui::Text("Max anisotropy: %.0fx   Extensions: %d", info.glMaxAnisotropy, info.glExtensionCount);

    ImGui::Separator();
    ImGui::Text("Display: %s", info.displayName.c_str());
    ImGui::Text("%d x %d @ %.0f Hz", info.displayWidth, info.displayHeight, info.displayRefreshRate);
}

void ConfigurationPanel::DrawSoftwareSection(Engine::Application& app)
{
    if (!ImGui::CollapsingHeader("Software versions", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    const Engine::SystemInfo& info = app.GetSystemInfo();
    ImGui::Text("SDL3: %s", info.sdlVersion.c_str());
    ImGui::Text("OpenGL: %s", info.openGlVersion.c_str());
    ImGui::Text("GLSL: %s", info.glslVersion.c_str());
    ImGui::Text("DevIL: %s", info.devilVersion.c_str());
    ImGui::Text("Assimp: %s", info.assimpVersion.c_str());
    ImGui::Text("GLM: %s", info.glmVersion.c_str());
    ImGui::Text("Dear ImGui: %s", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("Platform: %s   Video driver: %s", info.platform.c_str(), info.videoDriver.c_str());
}
} // namespace Editor
