#pragma once
#include "entity.hpp"
#include "../player/inventory.hpp"
#include <Graphics/2D/Sprite.hpp>

using namespace Stardust_Celeste;

class ItemDrop : public Entity {
    public:
    ItemDrop(RefPtr<Graphics::G2D::Sprite> model) { immortal = true; sprite = model; };
    virtual ~ItemDrop() = default;

    virtual auto update_drop(World* wrld, double dt, glm::vec3 player_pos) -> void;
    auto draw(float rot) -> void;

    Slot itemData;

    protected:
    RefPtr<Graphics::G2D::Sprite> sprite;
};
