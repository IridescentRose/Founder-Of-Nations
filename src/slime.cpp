#include "slime.hpp"

auto Slime::update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void {
    timer += dt;
    auto sinefunc = abs(sinf(timer * 2.0f));
    // (|sin x|) ^ 5
    // This is a basic bounce function
    auto func = sinefunc * sinefunc * sinefunc * sinefunc * sinefunc;

    pos.y = func;
    
    acceleration_speed = 15.0f * func;

    Enemy::update_enemy(wrld, dt, player_pos);
}