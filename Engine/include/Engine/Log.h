#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace Engine
{
// El motor registra mensajes; el editor se suscribe con un sink.
// Así el motor no conoce ImGui.
class Log
{
public:
    using Sink = std::function<void(std::string_view message)>;

    static void SetSink(Sink sink);
    static void Write(std::string_view message);
};
} // namespace Engine

#define ENGINE_LOG(msg) ::Engine::Log::Write(msg)
