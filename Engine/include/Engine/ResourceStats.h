#pragma once

#include <cstdint>

namespace Engine::ResourceStats
{
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
} 
