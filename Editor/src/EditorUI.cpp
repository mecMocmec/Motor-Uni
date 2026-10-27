#include "EditorUI.h"

#include <Engine/Application.h>
#include <Engine/Log.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <cstdint>

namespace Editor
{
EditorUI::EditorUI()
{
    // El sink es registra aviat per capturar també els logs d'inicialització del motor.
    Engine::Log::SetSink([this](std::string_view message) { m_consoleLines.emplace_back(message); });
}

EditorUI::~EditorUI()
{
    Engine::Log::SetSink(nullptr);
}

bool EditorUI::Init(Engine::Application& app)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForOpenGL(app.GetWindow(), app.GetGLContext()))
    {
        ENGINE_LOG("ImGui SDL3 backend init failed");
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 460"))
    {
        ENGINE_LOG("ImGui OpenGL3 backend init failed");
        return false;
    }

    ENGINE_LOGF("ImGui initialised (version {})", IMGUI_VERSION);
    m_initialised = true;
    return true;
}

void EditorUI::Shutdown()
{
    if (!m_initialised)
    {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    m_initialised = false;
}

void EditorUI::ProcessEvent(const SDL_Event& event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}

void EditorUI::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void EditorUI::Draw(Engine::Application& app)
{
    DrawMainMenu(app);
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    if (m_showScene)
    {
        DrawScenePanel(app);
    }
    if (m_showConsole)
    {
        DrawConsolePanel();
    }
}

void EditorUI::EndFrame(Engine::Application& app)
{
    ImGui::Render();
    app.ClearBackbuffer();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::DrawMainMenu(Engine::Application& app)
{
    if (!ImGui::BeginMainMenuBar())
    {
        return;
    }

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit"))
        {
            app.RequestQuit();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Escena", nullptr, &m_showScene);
        ImGui::MenuItem("Consola", nullptr, &m_showConsole);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void EditorUI::DrawScenePanel(Engine::Application& app)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Escena", &m_showScene))
    {
        const ImVec2 size = ImGui::GetContentRegionAvail();
        if (size.x > 1.0f && size.y > 1.0f)
        {
            Engine::Renderer& renderer = app.GetRenderer();
            renderer.ResizeTarget(static_cast<int>(size.x), static_cast<int>(size.y));
            renderer.RenderScene(app.TimeSeconds());

            const ImTextureID texture = static_cast<ImTextureID>(static_cast<std::uintptr_t>(renderer.TargetTexture()));
            // La textura d'OpenGL té l'origen a baix: invertim les UV en vertical.
            ImGui::Image(texture, size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorUI::DrawConsolePanel()
{
    if (ImGui::Begin("Consola", &m_showConsole))
    {
        if (ImGui::Button("Clear"))
        {
            m_consoleLines.clear();
        }
        ImGui::Separator();

        if (ImGui::BeginChild("ConsoleScroll", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
        {
            for (const std::string& line : m_consoleLines)
            {
                ImGui::TextUnformatted(line.c_str());
            }
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
} // namespace Editor
