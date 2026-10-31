#pragma once

#include <cstdint>
#include <string>

struct SDL_Window;

namespace Engine
{
// Detecció de hardware i versions del programari. Es consulta un cop, a l'inici.
struct SystemInfo
{
    // CPU i memòria del sistema
    std::string cpuName;
    int cpuLogicalCores = 0;
    int cpuCacheLineBytes = 0;
    std::string cpuFeatures;
    int systemRamMb = 0;

    // GPU i límits d'OpenGL
    std::string gpuVendor;
    std::string gpuRenderer;
    int glMaxTextureSize = 0;
    int glMaxTextureUnits = 0;
    int glMaxVertexAttribs = 0;
    int glExtensionCount = 0;
    float glMaxAnisotropy = 1.0f;

    // Pantalla
    std::string displayName;
    int displayWidth = 0;
    int displayHeight = 0;
    float displayRefreshRate = 0.0f;

    // Versions
    std::string platform;
    std::string videoDriver;
    std::string sdlVersion;
    std::string openGlVersion;
    std::string glslVersion;
    std::string devilVersion;
    std::string assimpVersion;
    std::string glmVersion;
};

// Consum de memòria en el moment de la crida (cal el context d'OpenGL actiu per a la part de GPU).
struct MemoryInfo
{
    std::uint64_t processWorkingSetBytes = 0;
    std::uint64_t processPeakWorkingSetBytes = 0;
    std::uint64_t processCommitBytes = 0;
    std::uint64_t systemTotalBytes = 0;
    std::uint64_t systemAvailableBytes = 0;

    bool gpuAvailable = false;       // NVX_gpu_memory_info o ATI_meminfo
    std::uint64_t gpuTotalKb = 0;    // 0 si el driver no la dona (ATI)
    std::uint64_t gpuFreeKb = 0;
};

SystemInfo QuerySystemInfo(SDL_Window* window);
MemoryInfo QueryMemoryInfo();
} // namespace Engine
