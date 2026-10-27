#pragma once

#include <format>
#include <functional>
#include <string>
#include <string_view>

namespace Engine
{
// El motor escribe mensajes; el editor se suscribe con un sink.
// Així el motor no coneix ImGui.
class Log
{
public:
    using Sink = std::function<void(std::string_view message)>;

    static void SetSink(Sink sink);
    static void Write(std::string_view message);
};
} // namespace Engine

#define ENGINE_LOG(msg)  ::Engine::Log::Write(msg)
#define ENGINE_LOGF(...) ::Engine::Log::Write(std::format(__VA_ARGS__))
