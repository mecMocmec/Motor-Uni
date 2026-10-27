#pragma once

#include "Engine/GameObject.h"

#include <memory>
#include <string>

namespace Engine
{
// Conté només el node arrel; la resta de GameObjects pengen d'ell com un arbre.
class Scene
{
public:
    Scene();

    GameObject& GetRoot() const { return *m_root; }

    // Si 'parent' és nullptr, el GameObject es crea sota l'arrel.
    GameObject& CreateGameObject(std::string name, GameObject* parent = nullptr);
    void Destroy(GameObject& gameObject);
    void Clear();

    void Update();

    GameObject* GetSelected() const { return m_selected; }
    void Select(GameObject* gameObject) { m_selected = gameObject; }

private:
    std::unique_ptr<GameObject> m_root;
    GameObject* m_selected = nullptr;
};
} // namespace Engine
