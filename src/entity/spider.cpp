#include "spider.hpp"

auto Spider::update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void {
    Enemy::update_enemy(wrld, dt, player_pos);
}