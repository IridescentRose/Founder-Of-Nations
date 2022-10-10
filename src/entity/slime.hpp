#pragma once
#include "enemy.hpp"

class Slime : public Enemy {
public:
	Slime(RefPtr<Graphics::G2D::Sprite> model) : timer(0.0f), Enemy(model) {
        acceleration_speed = 10.0f;
    };
    virtual ~Slime() = default;

    auto update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void override;
protected:
    float timer;
};