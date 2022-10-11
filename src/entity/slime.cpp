#include "slime.hpp"

auto Slime::update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void {
    timer += dt;
    auto sinefunc = fabsf(sinf(timer * 1.5f));
    // (|sin x|) ^ 25
    // This is a basic bounce function

    float func = 1.0f;
    for (int i = 0; i < 25; i++) {
        func *= sinefunc;
    }

    pos.y = func;
    
    acceleration_speed = 25.0f * func;

    Enemy::update_enemy(wrld, dt, player_pos);
}