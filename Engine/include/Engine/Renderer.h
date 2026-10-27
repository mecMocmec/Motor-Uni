#pragma once

#include <cstdint>

namespace Engine
{
class GameObject;
class Scene;

// Renderitza l'escena a un framebuffer (textura) que l'editor mostra a la finestra Escena.
class Renderer
{
public:
    bool Init();
    void Shutdown();

    void ResizeTarget(int width, int height);
    void RenderScene(const Scene& scene);

    std::uint32_t TargetTexture() const { return m_colorTex; }
    int TargetWidth() const { return m_width; }
    int TargetHeight() const { return m_height; }

private:
    void DestroyTarget();
    void DrawGameObject(const GameObject& gameObject) const;

    std::uint32_t m_program = 0;
    std::int32_t m_locModel = -1;
    std::int32_t m_locView = -1;
    std::int32_t m_locProjection = -1;

    std::uint32_t m_fbo = 0;
    std::uint32_t m_colorTex = 0;
    std::uint32_t m_depthRbo = 0;
    int m_width = 0;
    int m_height = 0;
};
} // namespace Engine
