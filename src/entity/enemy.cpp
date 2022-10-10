#include "enemy.hpp"

auto Enemy::update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void {
    glm::vec3 diff = player_pos - pos;
    float len = sqrtf(diff.x * diff.x + diff.z * diff.z);

    if(len > 1.0f)
        diff /= len;

    if(len < 32.0f)
        acc = diff * acceleration_speed;

    Entity::update(wrld, dt);
}

auto Enemy::draw(float rot) -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * 32.0f, pos.y * 32.0f, pos.z * 32.0f });
    Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    Rendering::RenderContext::get().matrix_translate({ (-0.5f) * 32.0f, 0, 0 });

    sprt->draw();
}