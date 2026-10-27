#include "Engine/Application.h"

#include "Engine/Log.h"
#include "GLInclude.h"

namespace Engine
{
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

    m_running = true;
    return true;
}

void Application::Shutdown()
{
    if (m_glContext != nullptr)
    {
        m_renderer.Shutdown();
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }
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
