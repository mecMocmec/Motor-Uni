#pragma once

#include <cstdint>

namespace Engine::ResourceStats
{
// Comptadors de recursos a la GPU (malles i textures vives i bytes aproximats).
struct Snapshot
{
    int meshCount = 0;
    std::uint64_t meshBytes = 0;
    int textureCount = 0;
    std::uint64_t textureBytes = 0;
};

Snapshot Get();

void OnMeshCreated(std::uint64_t bytes);
void OnMeshDestroyed(std::uint64_t bytes);
void OnTextureCreated(std::uint64_t bytes);
void OnTextureDestroyed(std::uint64_t bytes);
} // namespace Engine::ResourceStats
