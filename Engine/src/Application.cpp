#include "Engine/Application.h"

#include "Engine/ComponentMesh.h"
#include "Engine/Log.h"
#include "Engine/ModelImporter.h"
#include "Engine/Primitives.h"
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

// Extensió en minúscules, amb el punt (".fbx"); buida si no en té.
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
} // namespace

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
    LoadDefaultModel();

    m_running = true;
    return true;
}

void Application::Shutdown()
{
    if (m_glContext != nullptr)
    {
        // Les malles alliberen buffers d'OpenGL: cal destruir-les abans del context.
        m_scene.Clear();
        m_renderer.Shutdown();
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }
    ModelImporter::Shutdown();
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
        m_renderer.FocusOn(center, radius);
    }
    return true;
}

void Application::LoadDefaultModel()
{
    // Busca (també dins de subcarpetes) a Assets/Resources, al costat de l'executable,
    // un .fbx amb "baker" al nom.
    const char* basePath = SDL_GetBasePath();
    const std::filesystem::path resources = std::filesystem::path(basePath != nullptr ? basePath : "") / "Assets" / "Resources";
    ENGINE_LOGF("Looking for the default model in '{}'", ToUtf8(resources));

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
        ENGINE_LOG("Texture drag & drop will be available in the next step (DevIL)");
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
}

float Application::TimeSeconds() const
{
    return static_cast<float>(SDL_GetTicks()) / 1000.0f;
}
} // namespace Engine
