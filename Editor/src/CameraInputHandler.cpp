#include "CameraInputHandler.h"

namespace Editor
{
namespace
{
bool IsAltDown()
{
    const bool* keys = SDL_GetKeyboardState(nullptr);
    return keys[SDL_SCANCODE_LALT] || keys[SDL_SCANCODE_RALT];
}

bool IsShiftDown()
{
    const bool* keys = SDL_GetKeyboardState(nullptr);
    return keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
}
} // namespace

bool CameraInputHandler::ProcessEvent(const SDL_Event& event, SDL_Window* window, bool textInputActive)
{
    switch (event.type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        unsigned mask = 0;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            mask = IsAltDown() ? kLeft : 0; // el clic esquerre sol queda lliure (selecció)
            break;
        case SDL_BUTTON_MIDDLE:
            mask = kMiddle;
            break;
        case SDL_BUTTON_RIGHT:
            mask = kRight;
            break;
        default:
            break;
        }

        // Només s'inicia una captura si el clic és sobre l'Escena (o si ja n'hi havia una).
        if (mask != 0 && (m_sceneHovered || IsCapturing()))
        {
            const bool wasCapturing = IsCapturing();
            m_captureMask |= mask;
            if (!wasCapturing)
            {
                // Ratolí relatiu: cursor amagat i moviment il·limitat, com a Unity.
                SDL_SetWindowRelativeMouseMode(window, true);
            }
        }
        return false;
    }

    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
        unsigned mask = 0;
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mask = kLeft;
        }
        else if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            mask = kMiddle;
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            mask = kRight;
        }

        if (IsCapturing())
        {
            m_captureMask &= ~mask;
            if (!IsCapturing())
            {
                SDL_SetWindowRelativeMouseMode(window, false);
            }
        }
        return false;
    }

    case SDL_EVENT_MOUSE_MOTION:
        if (IsCapturing())
        {
            m_deltaX += event.motion.xrel;
            m_deltaY += event.motion.yrel;
            return true;
        }
        return false;

    case SDL_EVENT_MOUSE_WHEEL:
        if (m_sceneHovered || IsCapturing())
        {
            m_wheel += event.wheel.y;
        }
        return IsCapturing();

    case SDL_EVENT_KEY_DOWN:
        if (event.key.scancode == SDL_SCANCODE_F && !event.key.repeat && m_sceneHovered && !textInputActive && !IsCapturing())
        {
            m_focusRequested = true;
        }
        return false;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        ReleaseCapture(window);
        return false;

    default:
        return false;
    }
}

void CameraInputHandler::SetSceneState(bool hovered, float viewportHeight)
{
    m_sceneHovered = hovered;
    m_viewportHeight = viewportHeight;
}

Engine::CameraInput CameraInputHandler::BuildInput()
{
    Engine::CameraInput input;
    input.mouseDelta = glm::vec2(m_deltaX, m_deltaY);
    input.wheel = m_wheel;
    input.viewportHeight = m_viewportHeight;

    const bool alt = IsAltDown();
    input.flyActive = (m_captureMask & kRight) != 0 && !alt;
    input.dollyActive = (m_captureMask & kRight) != 0 && alt;
    input.orbitActive = (m_captureMask & kLeft) != 0 && alt;
    input.panActive = (m_captureMask & kMiddle) != 0;

    if (input.flyActive)
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        input.moveForward = keys[SDL_SCANCODE_W];
        input.moveBack = keys[SDL_SCANCODE_S];
        input.moveLeft = keys[SDL_SCANCODE_A];
        input.moveRight = keys[SDL_SCANCODE_D];
        input.moveUp = keys[SDL_SCANCODE_E];
        input.moveDown = keys[SDL_SCANCODE_Q];
    }
    input.fast = IsShiftDown();

    m_deltaX = 0.0f;
    m_deltaY = 0.0f;
    m_wheel = 0.0f;
    return input;
}

bool CameraInputHandler::ConsumeFocusRequest()
{
    const bool requested = m_focusRequested;
    m_focusRequested = false;
    return requested;
}

void CameraInputHandler::ReleaseCapture(SDL_Window* window)
{
    if (IsCapturing())
    {
        m_captureMask = 0;
        SDL_SetWindowRelativeMouseMode(window, false);
    }
}
} // namespace Editor
