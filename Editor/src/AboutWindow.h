#pragma once

namespace Engine
{
struct SystemInfo;
}

namespace Editor
{
class AboutWindow
{
public:
    void Draw(const Engine::SystemInfo& info, bool& open) const;
};
}
