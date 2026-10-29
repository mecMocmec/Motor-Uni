#include "EditorUI.h"

#include <Engine/Application.h>
#include <Engine/ComponentMesh.h>
#include <Engine/ComponentTexture.h>
#include <Engine/ComponentTransform.h>
#include <Engine/GameObject.h>
#include <Engine/Log.h>
#include <Engine/Primitives.h>
#include <Engine/Scene.h>
#include <Engine/Texture.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <cstdint>
#include <cstdio>

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
    if (m_showHierarchy)
    {
        DrawHierarchyPanel(app.GetScene());
    }
    if (m_showInspector)
    {
        DrawInspectorPanel(app.GetScene());
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
    if (ImGui::BeginMenu("GameObject"))
    {
        Engine::Scene& scene = app.GetScene();
        if (ImGui::MenuItem("Create Empty"))
        {
            scene.Select(&scene.CreateGameObject("GameObject"));
        }
        if (ImGui::MenuItem("Create Cube"))
        {
            Engine::GameObject& cube = scene.CreateGameObject("Cube");
            cube.AddComponent<Engine::ComponentMesh>(Engine::Primitives::CreateCube());
            scene.Select(&cube);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Escena", nullptr, &m_showScene);
        ImGui::MenuItem("Jerarquia", nullptr, &m_showHierarchy);
        ImGui::MenuItem("Inspector", nullptr, &m_showInspector);
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
            renderer.RenderScene(app.GetScene());

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

void EditorUI::DrawHierarchyPanel(Engine::Scene& scene)
{
    if (ImGui::Begin("Jerarquia", &m_showHierarchy))
    {
        // Les modificacions de l'arbre s'ajornen fins acabar de recórrer-lo.
        Engine::GameObject* toDelete = nullptr;
        Engine::GameObject* addChildTo = nullptr;

        for (const auto& child : scene.GetRoot().GetChildren())
        {
            DrawHierarchyNode(scene, *child, toDelete, addChildTo);
        }

        if (toDelete != nullptr)
        {
            scene.Destroy(*toDelete);
        }
        else if (addChildTo != nullptr)
        {
            scene.Select(&scene.CreateGameObject("GameObject", addChildTo));
        }

        if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                scene.Select(&scene.CreateGameObject("GameObject"));
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void EditorUI::DrawHierarchyNode(Engine::Scene& scene, Engine::GameObject& gameObject, Engine::GameObject*& toDelete,
                                 Engine::GameObject*& addChildTo)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (gameObject.GetChildren().empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (scene.GetSelected() == &gameObject)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    ImGui::PushID(&gameObject);

    const bool dimmed = !gameObject.IsActive();
    if (dimmed)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
    }
    const bool open = ImGui::TreeNodeEx(gameObject.GetName().c_str(), flags);
    if (dimmed)
    {
        ImGui::PopStyleColor();
    }

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        scene.Select(&gameObject);
    }

    if (ImGui::BeginPopupContextItem())
    {
        scene.Select(&gameObject);
        if (ImGui::MenuItem("Create Empty Child"))
        {
            addChildTo = &gameObject;
        }
        if (ImGui::MenuItem("Delete"))
        {
            toDelete = &gameObject;
        }
        ImGui::EndPopup();
    }

    if (open)
    {
        for (const auto& child : gameObject.GetChildren())
        {
            DrawHierarchyNode(scene, *child, toDelete, addChildTo);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void EditorUI::DrawInspectorPanel(Engine::Scene& scene)
{
    if (ImGui::Begin("Inspector", &m_showInspector))
    {
        Engine::GameObject* gameObject = scene.GetSelected();
        if (gameObject == nullptr)
        {
            ImGui::TextDisabled("No GameObject selected");
        }
        else
        {
            bool active = gameObject->IsActive();
            if (ImGui::Checkbox("##active", &active))
            {
                gameObject->SetActive(active);
            }
            ImGui::SameLine();

            char name[128];
            std::snprintf(name, sizeof(name), "%s", gameObject->GetName().c_str());
            ImGui::SetNextItemWidth(-1.0f);
            if (ImGui::InputText("##name", name, sizeof(name)))
            {
                gameObject->SetName(name);
            }

            ImGui::Separator();
            DrawTransformSection(*gameObject);
            DrawMeshSection(*gameObject);
            DrawTextureSection(*gameObject);
        }
    }
    ImGui::End();
}

void EditorUI::DrawTransformSection(Engine::GameObject& gameObject)
{
    if (!ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    Engine::ComponentTransform& transform = gameObject.GetTransform();

    glm::vec3 position = transform.GetPosition();
    if (ImGui::DragFloat3("Position", &position.x, 0.05f))
    {
        transform.SetPosition(position);
    }

    glm::vec3 rotation = transform.GetEulerDegrees();
    if (ImGui::DragFloat3("Rotation", &rotation.x, 0.5f))
    {
        transform.SetEulerDegrees(rotation);
    }

    glm::vec3 scale = transform.GetScale();
    if (ImGui::DragFloat3("Scale", &scale.x, 0.05f))
    {
        transform.SetScale(scale);
    }
}

void EditorUI::DrawMeshSection(Engine::GameObject& gameObject)
{
    Engine::ComponentMesh* meshComponent = gameObject.GetComponent<Engine::ComponentMesh>();
    if (meshComponent == nullptr || !ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    bool active = meshComponent->IsActive();
    if (ImGui::Checkbox("Active##mesh", &active))
    {
        meshComponent->SetActive(active);
    }

    const std::shared_ptr<Engine::Mesh>& mesh = meshComponent->GetMesh();
    if (mesh == nullptr)
    {
        ImGui::TextDisabled("No mesh assigned");
        return;
    }

    ImGui::Text("Name: %s", mesh->GetName().c_str());
    ImGui::Text("Vertices: %zu", mesh->GetVertices().size());
    ImGui::Text("Triangles: %zu", mesh->GetTriangleCount());
    ImGui::Text("AABB min: %.2f, %.2f, %.2f", mesh->GetAabbMin().x, mesh->GetAabbMin().y, mesh->GetAabbMin().z);
    ImGui::Text("AABB max: %.2f, %.2f, %.2f", mesh->GetAabbMax().x, mesh->GetAabbMax().y, mesh->GetAabbMax().z);
}

void EditorUI::DrawTextureSection(Engine::GameObject& gameObject)
{
    Engine::ComponentTexture* textureComponent = gameObject.GetComponent<Engine::ComponentTexture>();
    if (textureComponent == nullptr)
    {
        if (gameObject.GetComponent<Engine::ComponentMesh>() != nullptr && ImGui::Button("Add Texture component"))
        {
            gameObject.AddComponent<Engine::ComponentTexture>(std::shared_ptr<Engine::Texture>());
        }
        return;
    }
    if (!ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    bool active = textureComponent->IsActive();
    if (ImGui::Checkbox("Active##texture", &active))
    {
        textureComponent->SetActive(active);
    }

    bool checkers = textureComponent->UseCheckers();
    if (ImGui::Checkbox("Show checkers", &checkers))
    {
        textureComponent->SetUseCheckers(checkers);
    }

    const std::shared_ptr<Engine::Texture>& texture = textureComponent->GetTexture();
    if (texture == nullptr)
    {
        ImGui::TextDisabled("No texture assigned (drop a PNG/DDS on the window)");
        return;
    }

    ImGui::TextWrapped("Path: %s", texture->GetPath().c_str());
    ImGui::Text("Size: %d x %d", texture->GetWidth(), texture->GetHeight());

    const ImTextureID preview = static_cast<ImTextureID>(static_cast<std::uintptr_t>(texture->GetId()));
    ImGui::Image(preview, ImVec2(128.0f, 128.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
}
} // namespace Editor
