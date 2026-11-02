#pragma once

#include "Engine/Component.h"
#include "Engine/Texture.h"

#include <memory>
#include <utility>

namespace Engine
{
class ComponentTexture : public Component
{
public:
    static constexpr ComponentType kType = ComponentType::Texture;

    ComponentTexture(GameObject& owner, std::shared_ptr<Texture> texture) : Component(kType, owner), m_texture(std::move(texture)) {}

    const std::shared_ptr<Texture>& GetTexture() const { return m_texture; }
    void SetTexture(std::shared_ptr<Texture> texture) { m_texture = std::move(texture); }

    bool UseCheckers() const { return m_useCheckers; }
    void SetUseCheckers(bool useCheckers) { m_useCheckers = useCheckers; }

private:
    std::shared_ptr<Texture> m_texture;
    bool m_useCheckers = false;
};
} 
