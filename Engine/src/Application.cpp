#include "Engine/Application.h"

#include "Engine/ComponentMesh.h"
#include "Engine/ComponentTexture.h"
#include "Engine/Log.h"
#include "Engine/ModelImporter.h"
#include "Engine/Primitives.h"
#include "Engine/Texture.h"
#include "GLInclude.h"
#include "PathUtil.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <system_error>

namespace Engine
{
namespace
{
std::string ToLower(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

int ApplyTexture(GameObject& gameObject, const std::shared_ptr<Texture>& texture)
{
    int applied = 0;
    if (gameObject.GetComponent<ComponentMesh>() != nullptr)
    {
        if (ComponentTexture* component = gameObject.GetComponent<ComponentTexture>())
        {
            component->SetTexture(texture);
        }
        else
        {
            gameObject.AddComponent<ComponentTexture>(texture);
        }
        ++applied;
    }
    for (const auto& child : gameObject.GetChildren())
    {
        applied += ApplyTexture(*child, texture);
    }
    return applied;
}

std::string LowerExtension(const std::string& path)
{
    const std::size_t dot = path.find_last_of('.');
    const std::size_t separator = path.find_last_of("/\\");
    if (dot == std::string::npos || (separator != std::string::npos && dot < separator))
    {
        return {};
    }
    return ToLower(path.substr(dot));
}
} 

bool Application::Init(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        ENGINE_LOGF("SDL_Init failed: {}", SDL_GetError());
        return false;
    }
    ENGINE_LOGF("SDL initialised (version {}.{}.{})", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr)
    {
        ENGINE_LOGF("SDL_CreateWindow failed: {}", SDL_GetError());
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == nullptr)
    {
        ENGINE_LOGF("SDL_GL_CreateContext failed: {}", SDL_GetError());
        return false;
    }
    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1);
    int swapInterval = 0;
    SDL_GL_GetSwapInterval(&swapInterval);
    m_vsync = swapInterval != 0;

#if ENGINE_GLAD_V2
    const int version = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
#else
    const int version = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
#endif
    if (version == 0)
    {
        ENGINE_LOG("Error loading OpenGL functions with glad");
        return false;
    }

    ENGINE_LOGF("Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    ENGINE_LOGF("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    ENGINE_LOGF("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    ENGINE_LOGF("GLSL version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    if (!m_renderer.Init())
    {
        return false;
    }

    ModelImporter::Init();
    Texture::InitLoader();

    m_systemInfo = QuerySystemInfo(m_window);
    ENGINE_LOGF("CPU: {} ({} logical cores, {} MB RAM)", m_systemInfo.cpuName, m_systemInfo.cpuLogicalCores, m_systemInfo.systemRamMb);
    ENGINE_LOGF("CPU features: {}", m_systemInfo.cpuFeatures);
    ENGINE_LOGF("Display: {} {}x{} @ {:.0f} Hz", m_systemInfo.displayName, m_systemInfo.displayWidth, m_systemInfo.displayHeight,
                m_systemInfo.displayRefreshRate);
    ENGINE_LOGF("Libraries: SDL {}, DevIL {}, Assimp {}, GLM {}", m_systemInfo.sdlVersion, m_systemInfo.devilVersion, m_systemInfo.assimpVersion,
                m_systemInfo.glmVersion);
    LoadDefaultModel();

    m_lastTicksNs = SDL_GetTicksNS();
    m_running = true;
    return true;
}

void Application::Shutdown()
{
    if (m_glContext != nullptr)
    {
        m_scene.Clear();
        m_renderer.Shutdown();
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }
    ModelImporter::Shutdown();
    Texture::ShutdownLoader();
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
    ENGINE_LOG("Application shut down");
}

void Application::PumpEvents(const std::function<void(const SDL_Event&)>& onEvent)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (onEvent)
        {
            onEvent(event);
        }
        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            m_running = false;
        }
        else if (event.type == SDL_EVENT_DROP_FILE && event.drop.data != nullptr)
        {
            HandleDroppedFile(event.drop.data);
        }
    }
}

void Application::Update()
{
    const std::uint64_t now = SDL_GetTicksNS();
    m_deltaTime = std::min(static_cast<float>(now - m_lastTicksNs) / 1.0e9f, 0.1f);
    m_lastTicksNs = now;
    RecordFrame(m_deltaTime);

    m_scene.Update();
}

bool Application::LoadModel(const std::string& utf8Path)
{
    GameObject* root = ModelImporter::Load(m_scene, utf8Path);
    if (root == nullptr)
    {
        return false;
    }

    m_scene.Update();
    m_scene.Select(root);

    glm::vec3 boundsMin;
    glm::vec3 boundsMax;
    if (root->ComputeWorldAabb(boundsMin, boundsMax))
    {
        const glm::vec3 center = (boundsMin + boundsMax) * 0.5f;
        const float radius = glm::length(boundsMax - boundsMin) * 0.5f;
        m_editorCamera.Frame(center, radius, true, false);
    }
    return true;
}

void Application::LoadDefaultModel()
{
    const char* basePath = SDL_GetBasePath();
    const std::filesystem::path resources = std::filesystem::path(basePath != nullptr ? basePath : "") / "Assets" / "Resources";
    ENGINE_LOGF("Looking for the default model in '{}'", ToUtf8(resources));
    ModelImporter::SetResourcesRoot(ToUtf8(resources));

    std::error_code error;
    if (std::filesystem::is_directory(resources, error))
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(resources, error))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }
            const std::string fileName = ToLower(ToUtf8(entry.path().filename()));
            if (fileName.find("baker") != std::string::npos && LowerExtension(fileName) == ".fbx" && LoadModel(ToUtf8(entry.path())))
            {
                return;
            }
        }
    }

    ENGINE_LOG("Default model (baker_house .fbx) not found: creating a cube instead");
    GameObject& cube = m_scene.CreateGameObject("Cube");
    cube.AddComponent<ComponentMesh>(Primitives::CreateCube());
}

void Application::HandleDroppedFile(const std::string& utf8Path)
{
    ENGINE_LOGF("File dropped: {}", utf8Path);

    const std::string extension = LowerExtension(utf8Path);
    if (extension == ".fbx")
    {
        LoadModel(utf8Path);
    }
    else if (extension == ".png" || extension == ".dds")
    {
        GameObject* selected = m_scene.GetSelected();
        if (selected == nullptr)
        {
            ENGINE_LOG("Select a GameObject before dropping a texture");
            return;
        }
        if (const std::shared_ptr<Texture> texture = Texture::LoadFromFile(utf8Path))
        {
            ENGINE_LOGF("Texture applied to {} GameObject(s) under '{}'", ApplyTexture(*selected, texture), selected->GetName());
        }
    }
    else
    {
        ENGINE_LOGF("Unsupported file type '{}'", extension);
    }
}

void Application::ClearBackbuffer()
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(m_window, &width, &height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0.08f, 0.08f, 0.09f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::Present()
{
    SDL_GL_SwapWindow(m_window);
    LimitFrameRate();
}

void Application::RecordFrame(float deltaTime)
{
    if (deltaTime <= 0.0f)
    {
        return;
    }
    const float fps = 1.0f / deltaTime;
    m_frameHistory.fps[static_cast<std::size_t>(m_frameHistory.offset)] = fps;
    m_frameHistory.frameMs[static_cast<std::size_t>(m_frameHistory.offset)] = deltaTime * 1000.0f;
    m_frameHistory.offset = (m_frameHistory.offset + 1) % FrameHistory::kSamples;
    m_frameHistory.currentFps = m_frameHistory.currentFps <= 0.0f ? fps : m_frameHistory.currentFps + (fps - m_frameHistory.currentFps) * 0.1f;
}

void Application::LimitFrameRate()
{
    if (m_fpsLimit <= 0)
    {
        return;
    }
    const std::uint64_t targetNs = 1'000'000'000ull / static_cast<std::uint64_t>(m_fpsLimit);
    const std::uint64_t elapsedNs = SDL_GetTicksNS() - m_lastTicksNs;
    if (elapsedNs < targetNs)
    {
        SDL_DelayNS(targetNs - elapsedNs);
    }
}

void Application::GetWindowSize(int& width, int& height) const
{
    SDL_GetWindowSizeInPixels(m_window, &width, &height);
}

void Application::SetWindowSize(int width, int height)
{
    SDL_SetWindowSize(m_window, std::max(width, 320), std::max(height, 240));
}

bool Application::IsFullscreen() const
{
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
}

void Application::SetFullscreen(bool fullscreen)
{
    SDL_SetWindowFullscreen(m_window, fullscreen);
}

bool Application::IsResizable() const
{
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE) != 0;
}

void Application::SetResizable(bool resizable)
{
    SDL_SetWindowResizable(m_window, resizable);
}

bool Application::IsBordered() const
{
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_BORDERLESS) == 0;
}

void Application::SetBordered(bool bordered)
{
    SDL_SetWindowBordered(m_window, bordered);
}

void Application::SetVSync(bool vsync)
{
    if (SDL_GL_SetSwapInterval(vsync ? 1 : 0))
    {
        m_vsync = vsync;
    }
    else
    {
        ENGINE_LOGF("Could not change VSync: {}", SDL_GetError());
    }
}

MemoryInfo Application::QueryMemory() const
{
    return QueryMemoryInfo();
}

float Application::TimeSeconds() const
{
    return static_cast<float>(SDL_GetTicks()) / 1000.0f;
}
}
