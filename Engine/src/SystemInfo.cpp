#include "Engine/SystemInfo.h"

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>

    #include <psapi.h>
#endif

#include "GLInclude.h"

#include <SDL3/SDL.h>

#include <IL/il.h>
#include <assimp/version.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <cstring>
#include <format>

namespace Engine
{
namespace
{
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY
    #define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#endif

constexpr unsigned int kNvxTotalAvailableKb = 0x9048;
constexpr unsigned int kNvxCurrentAvailableKb = 0x9049;
constexpr unsigned int kAtiVboFreeMemory = 0x87FB;

std::string GlString(unsigned int name)
{
    const auto* text = reinterpret_cast<const char*>(glGetString(name));
    return text != nullptr ? text : "unknown";
}

bool HasGlExtension(const char* extension)
{
    GLint count = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);
    for (GLint i = 0; i < count; ++i)
    {
        const auto* name = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));
        if (name != nullptr && std::strcmp(name, extension) == 0)
        {
            return true;
        }
    }
    return false;
}

std::string ReadCpuName()
{
#ifdef _WIN32
    char buffer[256] = {};
    DWORD size = sizeof(buffer);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_REG_SZ, nullptr, buffer,
                     &size) == ERROR_SUCCESS)
    {
        std::string name(buffer);
        const std::size_t first = name.find_first_not_of(' ');
        return first == std::string::npos ? name : name.substr(first);
    }
#endif
    return "unknown";
}

std::string ReadCpuFeatures()
{
    std::string features;
    const auto add = [&features](bool present, const char* name)
    {
        if (present)
        {
            features += features.empty() ? "" : " ";
            features += name;
        }
    };
    add(SDL_HasSSE(), "SSE");
    add(SDL_HasSSE2(), "SSE2");
    add(SDL_HasSSE3(), "SSE3");
    add(SDL_HasSSE41(), "SSE4.1");
    add(SDL_HasSSE42(), "SSE4.2");
    add(SDL_HasAVX(), "AVX");
    add(SDL_HasAVX2(), "AVX2");
    return features.empty() ? "none detected" : features;
}
} // namespace

SystemInfo QuerySystemInfo(SDL_Window* window)
{
    SystemInfo info;

    info.cpuName = ReadCpuName();
    info.cpuLogicalCores = SDL_GetNumLogicalCPUCores();
    info.cpuCacheLineBytes = SDL_GetCPUCacheLineSize();
    info.cpuFeatures = ReadCpuFeatures();
    info.systemRamMb = SDL_GetSystemRAM();

    info.gpuVendor = GlString(GL_VENDOR);
    info.gpuRenderer = GlString(GL_RENDERER);
    info.openGlVersion = GlString(GL_VERSION);
    info.glslVersion = GlString(GL_SHADING_LANGUAGE_VERSION);

    GLint value = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
    info.glMaxTextureSize = value;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    info.glMaxTextureUnits = value;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    info.glMaxVertexAttribs = value;
    glGetIntegerv(GL_NUM_EXTENSIONS, &value);
    info.glExtensionCount = value;

    while (glGetError() != GL_NO_ERROR)
    {
    }
    GLfloat anisotropy = 1.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisotropy);
    info.glMaxAnisotropy = (glGetError() == GL_NO_ERROR && anisotropy >= 1.0f) ? anisotropy : 1.0f;

    const SDL_DisplayID display = SDL_GetDisplayForWindow(window);
    if (const char* name = SDL_GetDisplayName(display))
    {
        info.displayName = name;
    }
    if (const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display))
    {
        info.displayWidth = mode->w;
        info.displayHeight = mode->h;
        info.displayRefreshRate = mode->refresh_rate;
    }

    info.platform = SDL_GetPlatform();
    if (const char* driver = SDL_GetCurrentVideoDriver())
    {
        info.videoDriver = driver;
    }

    const int sdl = SDL_GetVersion();
    info.sdlVersion = std::format("{}.{}.{}", SDL_VERSIONNUM_MAJOR(sdl), SDL_VERSIONNUM_MINOR(sdl), SDL_VERSIONNUM_MICRO(sdl));

    const int devil = ilGetInteger(IL_VERSION_NUM); // p. ex. 180 = 1.8.0
    info.devilVersion = std::format("{}.{}.{}", devil / 100, (devil / 10) % 10, devil % 10);

    info.assimpVersion = std::format("{}.{}.{}", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionPatch());
    info.glmVersion = std::format("{}.{}.{}", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);
    return info;
}

MemoryInfo QueryMemoryInfo()
{
    MemoryInfo info;

#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS counters{};
    counters.cb = sizeof(counters);
    if (GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters)))
    {
        info.processWorkingSetBytes = counters.WorkingSetSize;
        info.processPeakWorkingSetBytes = counters.PeakWorkingSetSize;
        info.processCommitBytes = counters.PagefileUsage;
    }

    MEMORYSTATUSEX status{};
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status))
    {
        info.systemTotalBytes = status.ullTotalPhys;
        info.systemAvailableBytes = status.ullAvailPhys;
    }
#else
    info.systemTotalBytes = static_cast<std::uint64_t>(SDL_GetSystemRAM()) * 1024ull * 1024ull;
#endif

    // Memòria de la GPU: només amb les extensions de NVIDIA o AMD. Es comprova un cop.
    static const bool hasNvx = HasGlExtension("GL_NVX_gpu_memory_info");
    static const bool hasAti = !hasNvx && HasGlExtension("GL_ATI_meminfo");
    while (glGetError() != GL_NO_ERROR)
    {
    }
    if (hasNvx)
    {
        GLint total = 0;
        GLint current = 0;
        glGetIntegerv(kNvxTotalAvailableKb, &total);
        glGetIntegerv(kNvxCurrentAvailableKb, &current);
        info.gpuAvailable = glGetError() == GL_NO_ERROR;
        info.gpuTotalKb = static_cast<std::uint64_t>(std::max(total, 0));
        info.gpuFreeKb = static_cast<std::uint64_t>(std::max(current, 0));
    }
    else if (hasAti)
    {
        GLint free[4] = {};
        glGetIntegerv(kAtiVboFreeMemory, free);
        info.gpuAvailable = glGetError() == GL_NO_ERROR;
        info.gpuFreeKb = static_cast<std::uint64_t>(std::max(free[0], 0));
    }
    return info;
}
} // namespace Engine
