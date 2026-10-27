#pragma once

#include <filesystem>
#include <string>

namespace Engine
{
// Les rutes dins del motor són std::string en UTF-8 (és el que donen SDL i Assimp).
inline std::string ToUtf8(const std::filesystem::path& path)
{
    const std::u8string text = path.u8string();
    return std::string(reinterpret_cast<const char*>(text.data()), text.size());
}

inline std::filesystem::path FromUtf8(const std::string& text)
{
    return std::filesystem::path(std::u8string(reinterpret_cast<const char8_t*>(text.data()), text.size()));
}
} // namespace Engine
