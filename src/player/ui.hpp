#pragma once

#include <Stardust-Celeste.hpp>
#include <Rendering/Camera.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Graphics/2D/FontRenderer.hpp>
#include "../entity/entity.hpp"
#include "inventory.hpp"

using namespace Stardust_Celeste;

class UI final {
    public:
    UI(Entity& e);

    auto draw(RefPtr<Inventory> inv, bool inInv) -> void;

    u32 slot_sel;

    private:
    Entity& entity;
    u32 texID, fontID, itemID;
    ScopePtr<Graphics::G2D::Sprite> background_bar;
    ScopePtr<Graphics::G2D::Sprite> red_bar;
    ScopePtr<Graphics::G2D::Sprite> blue_bar;
    ScopePtr<Graphics::G2D::Sprite> green_bar;
    ScopePtr<Graphics::G2D::Sprite> item_slot;
    ScopePtr<Graphics::G2D::Sprite> item_slot_sel;
    ScopePtr<Graphics::G2D::Sprite> items[64];
    ScopePtr<Graphics::G2D::FontRenderer> font_renderer;
};