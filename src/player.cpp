#include "player.hpp"
#include "tiles.hpp"

auto degtorad(float x) -> float {
    return x / 180.0f * 3.14159f;
}

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

    vel = glm::vec3(0, 0, 0);
    acc = glm::vec3(0, 0, 0);
    pos = glm::vec3(1024 + 8, 0, 1024 + 8);
    rot = 0.0f;
}

Player::~Player() {

}

auto test(glm::ivec2 pos, World* wrld) -> bool {
    auto t = wrld->get_tile(pos);
    auto l = wrld->get_tile2(pos);
    return t == Tile::Stone || t == Tile::Stone_Coal || l == 1;
}

const float PLAYER_ACCELERATION = 42.069f;
auto Player::update(World* wrld, double dt) -> void {
    float len = sqrtf(acc.x * acc.x + acc.z * acc.z);

    if(len > 0) {
        acc /= len;
        acc *= PLAYER_ACCELERATION;
    }

    vel += acc * (float)dt;
    auto testpos = pos + vel * (float)dt;

    {
        int x, z;
        bool testX = false;
        bool testZ = false;

        int xMin = (int)(pos.x - 0.3f);
        int xMax = (int)(pos.x + 0.3f);
        int zMin = (int)(pos.z - 0.3f);
        int zMax = (int)(pos.z + 0.3f);

        if (vel.x < 0.0f) {
            x = (int)(pos.x - 0.3f + vel.x * dt);
            testX = true;
        }
        else if (vel.x > 0.0f) {
            x = (int)(pos.x + 0.3f + vel.x * dt);
            testX = true;
        }

        if (vel.z < 0.0f) {
            z = (int)(pos.z - 0.3f + vel.z * dt);
            testZ = true;
        }
        else if (vel.z > 0.0f) {
            z = (int)(pos.z + 0.3f + vel.z * dt);
            testZ = true;
        }

        if (testX) {
            for (int z = zMin; z <= zMax; z++) {
                glm::ivec2 p = glm::ivec2(x, z);
                if (test(p, wrld)) {
                    vel.x = 0;
                    break;
                }
            }
        }
        if (testZ) {
            for (int x = xMin; x <= xMax; x++) {
                glm::ivec2 p = glm::ivec2(x, z);
                if (test(p, wrld)) {
                    vel.z = 0;
                    break;
                }
            }
        }
    }

    pos += vel * (float)dt;

    camera->pos = pos * PLAYER_SIZE;
    camera->pos.y += 4.2f * PLAYER_SIZE;
    camera->pos.x += sinf(degtorad(rot)) * 6.0f * PLAYER_SIZE;
    camera->pos.z += cosf(degtorad(rot)) * 6.0f * PLAYER_SIZE;

    camera->rot.y = degtorad(-rot);

    vel *= 0.9f;
    acc = glm::vec3(0, 0, 0);

    camera->update();
}

auto Player::draw() -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * PLAYER_SIZE, pos.y * PLAYER_SIZE, pos.z * PLAYER_SIZE });
    Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    Rendering::RenderContext::get().matrix_translate({ (-0.5f) * PLAYER_SIZE, 0, 0 });

    character->draw();
}

auto Player::move_up(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->acc.x += -sinf(degtorad(p->rot)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_down(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->acc.x += sinf(degtorad(p->rot)) * PLAYER_ACCELERATION;
        p->acc.z += cosf(degtorad(p->rot)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_left(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->acc.x += -sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_right(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->acc.x += sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}

auto Player::move_tiltL(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->rot += 1.0f;
    }
}

auto Player::move_tiltR(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->rot -= 1.0f;
    }
}