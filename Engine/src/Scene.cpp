#include "Engine/Scene.h"

#include "Engine/Log.h"

namespace Engine
{
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

void Scene::Update()
{
    m_root->UpdateTransforms(glm::mat4(1.0f));
}
} // namespace Engine
