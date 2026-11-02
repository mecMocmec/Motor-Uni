#include "Engine/Scene.h"

#include "Engine/ComponentMesh.h"
#include "Engine/Log.h"

namespace Engine
{
namespace
{
void Accumulate(const GameObject& gameObject, SceneStats& stats)
{
    ++stats.gameObjects;
    if (const ComponentMesh* meshComponent = gameObject.GetComponent<ComponentMesh>())
    {
        if (meshComponent->GetMesh() != nullptr)
        {
            ++stats.meshComponents;
            stats.vertices += meshComponent->GetMesh()->GetVertices().size();
            stats.triangles += meshComponent->GetMesh()->GetTriangleCount();
        }
    }
    for (const auto& child : gameObject.GetChildren())
    {
        Accumulate(*child, stats);
    }
}
} 

Scene::Scene() : m_root(std::make_unique<GameObject>("Root")) {}

GameObject& Scene::CreateGameObject(std::string name, GameObject* parent)
{
    GameObject& owner = (parent != nullptr) ? *parent : *m_root;
    GameObject& created = owner.CreateChild(std::move(name));
    ENGINE_LOGF("GameObject '{}' created", created.GetName());
    return created;
}

void Scene::Destroy(GameObject& gameObject)
{
    if (&gameObject == m_root.get() || gameObject.GetParent() == nullptr)
    {
        return;
    }
    if (m_selected == &gameObject || (m_selected != nullptr && m_selected->IsDescendantOf(&gameObject)))
    {
        m_selected = nullptr;
    }
    ENGINE_LOGF("GameObject '{}' destroyed", gameObject.GetName());
    gameObject.GetParent()->RemoveChild(&gameObject);
}

void Scene::Clear()
{
    m_selected = nullptr;
    m_root = std::make_unique<GameObject>("Root");
}

SceneStats Scene::ComputeStats() const
{
    SceneStats stats;
    for (const auto& child : m_root->GetChildren())
    {
        Accumulate(*child, stats);
    }
    return stats;
}

void Scene::Update()
{
    m_root->UpdateTransforms(glm::mat4(1.0f));
}
}
