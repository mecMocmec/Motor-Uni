#pragma once

#include "AboutWindow.h"
#include "CameraInputHandler.h"
#include "ConfigurationPanel.h"

#include <SDL3/SDL.h>

#include <string>
#include <vector>

namespace Engine
{
class Application;
class GameObject;
class Scene;
}

namespace Editor
{
class EditorUI
{
public:
    EditorUI();
    ~EditorUI();

    bool Init(Engine::Application& app);
    void Shutdown();

    void ProcessEvent(const SDL_Event& event);
    void BeginFrame();
    void Draw(Engine::Application& app);
    void EndFrame(Engine::Application& app);

private:
    void UpdateCamera(Engine::Application& app);
    void FocusSelection(Engine::Application& app);
    void DrawMainMenu(Engine::Application& app);
    void DrawScenePanel(Engine::Application& app);
    void DrawConsolePanel();
    void DrawHierarchyPanel(Engine::Scene& scene);
    void DrawHierarchyNode(Engine::Scene& scene, Engine::GameObject& gameObject, Engine::GameObject*& toDelete,
                           Engine::GameObject*& addChildTo);
    void DrawInspectorPanel(Engine::Scene& scene);
    void DrawTransformSection(Engine::GameObject& gameObject);
    void DrawMeshSection(Engine::GameObject& gameObject);
    void DrawTextureSection(Engine::GameObject& gameObject);

    std::vector<std::string> m_consoleLines;
    CameraInputHandler m_cameraInput;
    ConfigurationPanel m_configurationPanel;
    AboutWindow m_aboutWindow;
    SDL_Window* m_window = nullptr;
    bool m_showScene = true;
    bool m_showConsole = true;
    bool m_showConfiguration = true;
    bool m_showAbout = false;
    bool m_showHierarchy = true;
    bool m_showInspector = true;
    bool m_initialised = false;
};
}
