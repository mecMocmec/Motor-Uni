#pragma once

#include "Engine/Mesh.h"

#include <memory>

namespace Engine::Primitives
{
// Cub unitari centrat a l'origen (24 vèrtexs, una normal per cara).
std::shared_ptr<Mesh> CreateCube();
} // namespace Engine::Primitives
