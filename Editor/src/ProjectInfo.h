#pragma once

namespace Editor::ProjectInfo
{
struct Member
{
    const char* name;
    const char* github;
};

inline constexpr const char* kEngineName = "Motor";
inline constexpr const char* kVersion = "1.0";
inline constexpr const char* kShortDescription = "Geometry viewer with an ImGui editor made for the Motors de Videojocs subject";

inline constexpr const char* kRepositoryUrl = "https://github.com/mecMocmec/Motor-Uni";

inline constexpr Member kMembers[] = {
    {"Elena Ibañez Armora", "mecMocmec"},
    {"Marcel Pascual Martinez", "guifre1714"},
    {"Marc Port Prat", "Marcoto06"},
};
bool OpenRepositoryPage(const char* path);
}
