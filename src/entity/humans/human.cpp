#include "human.hpp"


Human::Human(u32 texture, u8 type) : hType(type), facing(false) {
    sprt = create_refptr<Graphics::G2D::AnimatedSprite>(
        texture,
        Rendering::Rectangle{ {0, 0}, {50 * 1.5f, 37 * 1.5f}},
        glm::vec2(10, 5)
    );
    sprt->set_animation_range(0 + hType * 10, 3 + hType * 10);

    moveTimer = 0.0f;
    nextMoveTime = 0.0f;
}

Human::~Human() {
    sprt.reset();
}

auto Human::update_human(World* wrld, double dt, glm::vec3 player_pos) -> void {

    glm::vec3 diff = player_pos - pos;
    float len = sqrtf(diff.x * diff.x + diff.z * diff.z);
    if (len > 1.0f)
        diff /= len;

    if (hType == HUMAN_TYPE_BANDIT && len < 6.0f) {
        acc = -diff * 25.0f;
    }
    else {

        moveTimer += dt;

        if (moveTimer > nextMoveTime) {
            nextMoveTime = 4 + rand() % 7;
            moveTimer = 0;

            moving = (rand() % 2) == 0;

            if (moving) {
                auto rx = rand() % 3 - 1;
                auto rz = rand() % 3 - 1;
                set_acc = glm::vec3(rx * 10.0f, 0.0f, rz * 10.0f);
            }
        }

        if (moving) {
            acc = set_acc;
        }
        else {
            acc = glm::vec3(0);
            vel = glm::vec3(0);
        }
    }

    if (vel.x != 0 || vel.z != 0) {
        sprt->set_animation_range(4 + hType * 10, 9 + hType * 10);
    }
    else {
        sprt->set_animation_range(0 + hType * 10, 3 + hType * 10);
    }

    facing = vel.x > 0;

    sprt->update(dt);

    Entity::update(wrld, dt);
}

const float HUMAN_SIZE = 32.0f;
auto Human::draw(float rot) -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * HUMAN_SIZE, pos.y * HUMAN_SIZE, pos.z * HUMAN_SIZE });

    glDisable(GL_CULL_FACE);
    auto degrees = (int)rot % 360;

    auto facingTemp = facing;
    if(degrees > 90 && degrees < 270)
        facingTemp = !facingTemp;

    if(facingTemp)
        Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    else 
        Rendering::RenderContext::get().matrix_rotate({ 0, rot + 180, 0 });
    
    Rendering::RenderContext::get().matrix_translate({ -50*0.75f, 0, 0 });
    sprt->draw();

    glEnable(GL_CULL_FACE);
}