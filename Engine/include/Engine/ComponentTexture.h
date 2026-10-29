#pragma once

#include "Engine/Component.h"
#include "Engine/Texture.h"

#include <memory>
#include <utility>

namespace Engine
{
// Textura difusa d'un GameObject. Pot no tenir textura assignada i servir només per veure els quadrats.
class ComponentTexture : public Component
{
public:
    static constexpr ComponentType kType = ComponentType::Texture;

    ComponentTexture(GameObject& owner, std::shared_ptr<Texture> texture) : Component(kType, owner), m_texture(std::move(texture)) {}

    const std::shared_ptr<Texture>& GetTexture() const { return m_texture; }
    void SetTexture(std::shared_ptr<Texture> texture) { m_texture = std::move(texture); }

    // Substitueix la textura per la de quadrats blancs i negres (per comprovar les UV).
    bool UseCheckers() const { return m_useCheckers; }
    void SetUseCheckers(bool useCheckers) { m_useCheckers = useCheckers; }

private:
    std::shared_ptr<Texture> m_texture;
    bool m_useCheckers = false;
};
} // namespace Engine
