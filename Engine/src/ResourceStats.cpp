#include "Engine/ResourceStats.h"

namespace Engine::ResourceStats
{
namespace
{
Snapshot g_stats;
}

Snapshot Get()
{
    return g_stats;
}

void OnMeshCreated(std::uint64_t bytes)
{
    ++g_stats.meshCount;
    g_stats.meshBytes += bytes;
}

void OnMeshDestroyed(std::uint64_t bytes)
{
    --g_stats.meshCount;
    g_stats.meshBytes -= bytes;
}

void OnTextureCreated(std::uint64_t bytes)
{
    ++g_stats.textureCount;
    g_stats.textureBytes += bytes;
}

void OnTextureDestroyed(std::uint64_t bytes)
{
    --g_stats.textureCount;
    g_stats.textureBytes -= bytes;
}
} // namespace Engine::ResourceStats
