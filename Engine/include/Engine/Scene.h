#pragma once

#include "Engine/GameObject.h"

#include <cstdint>
#include <memory>
#include <string>

namespace Engine
{
struct SceneStats
{
    int gameObjects = 0;
    int meshComponents = 0;
    std::uint64_t vertices = 0;
    std::uint64_t triangles = 0;
};

class Scene
{
public:
    Scene();

    GameObject& GetRoot() const { return *m_root; }

    GameObject& CreateGameObject(std::string name, GameObject* parent = nullptr);
    void Destroy(GameObject& gameObject);
    void Clear();

    void Update();
    SceneStats ComputeStats() const;

    GameObject* GetSelected() const { return m_selected; }
    void Select(GameObject* gameObject) { m_selected = gameObject; }

private:
    std::unique_ptr<GameObject> m_root;
    GameObject* m_selected = nullptr;
};
} 
