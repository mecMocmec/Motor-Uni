#pragma once

#include <string>

namespace Engine
{
class GameObject;
class Scene;


namespace ModelImporter
{
void Init();
void Shutdown();

void SetResourcesRoot(const std::string& utf8Path);

GameObject* Load(Scene& scene, const std::string& utf8Path, GameObject* parent = nullptr);
} 
} 
