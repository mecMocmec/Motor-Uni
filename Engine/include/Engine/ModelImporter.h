#pragma once

#include <string>

namespace Engine
{
class GameObject;
class Scene;

// Única classe que coneix Assimp: la resta del motor no en veu cap rastre.
namespace ModelImporter
{
void Init();
void Shutdown();

// Importa el fitxer (ruta UTF-8) i crea la jerarquia de GameObjects sota 'parent'
// (o sota l'arrel si és nullptr). Retorna el GameObject arrel del model, o nullptr si falla.
GameObject* Load(Scene& scene, const std::string& utf8Path, GameObject* parent = nullptr);
} // namespace ModelImporter
} // namespace Engine
