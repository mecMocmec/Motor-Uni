#include "EditorUI.h"

#include <Engine/Application.h>

int main()
{
    Editor::EditorUI ui;
    Engine::Application app;

    if (!app.Init("Motor", 1280, 720))
    {
        app.Shutdown();
        return 1;
    }
    if (!ui.Init(app))
    {
        ui.Shutdown();
        app.Shutdown();
        return 1;
    }

    while (app.IsRunning())
    {
        app.PumpEvents([&ui](const SDL_Event& event) { ui.ProcessEvent(event); });
        app.Update();

        ui.BeginFrame();
        ui.Draw(app);
        ui.EndFrame(app);

        app.Present();
    }

    ui.Shutdown();
    app.Shutdown();
    return 0;
}
