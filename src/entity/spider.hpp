#pragma once
#include "enemy.hpp"

class Spider : public Enemy {
public:
	Spider(RefPtr<Graphics::G2D::Sprite> model) : Enemy(model) {
        acceleration_speed = 30.0f;
    };
    virtual ~Spider() = default;

    auto update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void override;
protected:
};