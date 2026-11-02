#pragma once

#include <format>
#include <functional>
#include <string>
#include <string_view>

namespace Engine
{

class Log
{
public:
    using Sink = std::function<void(std::string_view message)>;

    static void SetSink(Sink sink);
    static void Write(std::string_view message);
};
} 

#define ENGINE_LOG(msg)  ::Engine::Log::Write(msg)
#define ENGINE_LOGF(...) ::Engine::Log::Write(std::format(__VA_ARGS__))
