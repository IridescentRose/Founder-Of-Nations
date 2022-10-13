#pragma once
#include "enemy.hpp"

class Beholder : public Enemy {
public:
	Beholder(RefPtr<Graphics::G2D::Sprite> model) : Enemy(model) {
        acceleration_speed = 10.0f;
    };
    virtual ~Beholder() = default;
};