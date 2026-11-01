#pragma once

#include "Engine/Mesh.h"

#include <memory>

namespace Engine::Primitives
{
std::shared_ptr<Mesh> CreateCube();

std::shared_ptr<Mesh> CreateSphere();

std::shared_ptr<Mesh> CreatePyramid();

std::shared_ptr<Mesh> CreateCylinder();
}
