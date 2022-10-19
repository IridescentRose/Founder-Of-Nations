#pragma once
#include "../entity.hpp"
#include <Graphics/2D/AnimatedSprite.hpp>
#include <map>
using namespace Stardust_Celeste;

enum HumanType {
    HUMAN_TYPE_CIVILIAN,
    HUMAN_TYPE_TRADER,
    HUMAN_TYPE_BANDIT,
    HUMAN_TYPE_GUARD
};

class Enemy;

class Human : public Entity {
public:
    Human(u32 texture, u8 type);
    virtual ~Human();

    virtual auto update_human(World* wrld, double dt, glm::vec3 player_pos, const std::map<u32, RefPtr<Enemy>>& enemies) -> void;
    auto draw(float rot) -> void;
    u8 hType;
protected:
    float moveTimer;
    float nextMoveTime;
    bool moving;
    glm::vec3 set_acc;
    bool facing;
    RefPtr<Graphics::G2D::AnimatedSprite> sprt;
};
