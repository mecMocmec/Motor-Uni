#include "Engine/Log.h"

#include <cstdio>
#include <utility>

namespace Engine
{
namespace
{
Log::Sink g_sink;
}

void Log::SetSink(Sink sink)
{
    g_sink = std::move(sink);
}

void Log::Write(std::string_view message)
{
    std::fprintf(stdout, "%.*s\n", static_cast<int>(message.size()), message.data());
    if (g_sink)
    {
        g_sink(message);
    }
}
}