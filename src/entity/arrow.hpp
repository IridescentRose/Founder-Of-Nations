#pragma once
#include "entity.hpp"
#include "../player/inventory.hpp"
#include <Graphics/2D/Sprite.hpp>

using namespace Stardust_Celeste;

class ArrowEntity : public Entity {
    public:
        ArrowEntity(RefPtr<Graphics::G2D::Sprite> model, glm::vec3 vel) { timer = 0.0f; fixedVel = vel; immortal = true; sprite = model; };
    virtual ~ArrowEntity() = default;

    auto update_arrow(World* wrld, double dt) -> void;
    auto draw(float rot) -> void;

    bool facing;
    float timer;
    protected:
    glm::vec3 fixedVel;
    RefPtr<Graphics::G2D::Sprite> sprite;
};
