#include "player.hpp"
#include "tiles.hpp"

Player::Player() : texture(0) {
	texture = Rendering::TextureManager::get().load_texture("./assets/char.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);

    camera = create_scopeptr<Rendering::Camera>(
        glm::vec3(0, 0, 0),
        glm::vec3(degtorad(30.0f), 0, 0),
        degtorad(45.0f),
        16.0f / 9.0f,
        0.3f,
        128.0f * PLAYER_SIZE
    );

    Rendering::RenderContext::get().set_mode_3D();

    character = create_scopeptr<Graphics::G2D::Sprite>(
        texture,
        Rendering::Rectangle{ {0, 0}, {PLAYER_SIZE, PLAYER_SIZE} }
    );

    ui = create_scopeptr<UI>(*this);

    rot = 0.0f;
    pos = glm::vec3(1024 + 8, 0, 1024 + 8);

    hp = base_hp = 100;
    energy = base_energy = 100;
    xp = 10;
}

Player::~Player() {

}

auto Player::update(World* wrld, double dt) -> void {
    float len = sqrtf(acc.x * acc.x + acc.z * acc.z);

    if(len > 0) {
        acc /= len;
        acc *= PLAYER_ACCELERATION;
    }

    Entity::update(wrld, dt);

    camera->pos = pos * PLAYER_SIZE;
    camera->pos.y += 4.2f * PLAYER_SIZE;
    camera->pos.x += sinf(degtorad(rot)) * 6.0f * PLAYER_SIZE;
    camera->pos.z += cosf(degtorad(rot)) * 6.0f * PLAYER_SIZE;
    camera->rot.y = degtorad(-rot);
    Rendering::RenderContext::get().set_mode_3D();
    camera->update();
}

auto Player::draw() -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * PLAYER_SIZE, pos.y * PLAYER_SIZE, pos.z * PLAYER_SIZE });
    Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    Rendering::RenderContext::get().matrix_translate({ (-0.5f) * PLAYER_SIZE, 0, 0 });

    character->draw();

    Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -30, 30);
    Rendering::RenderContext::get().set_mode_2D();
    Rendering::RenderContext::get().matrix_clear();
    ui->draw();
}
