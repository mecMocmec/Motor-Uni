#include "ProjectInfo.h"

#include <Engine/Log.h>

#include <SDL3/SDL.h>

#include <string>

namespace Editor::ProjectInfo
{
bool OpenRepositoryPage(const char* path)
{
    const std::string url = std::string(kRepositoryUrl) + path;

    if (url.find('<') != std::string::npos)
    {
        ENGINE_LOGF("Set kRepositoryUrl in ProjectInfo.h before using the Help links (now: {})", url);
        return false;
    }

    ENGINE_LOGF("Opening {}", url);
    if (!SDL_OpenURL(url.c_str()))
    {
        ENGINE_LOGF("Could not open the browser: {}", SDL_GetError());
        return false;
    }
    return true;
}
}
