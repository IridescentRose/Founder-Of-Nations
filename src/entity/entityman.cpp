#include "entityman.hpp"
#include "../entity/enemy.hpp"
#include "../entity/slime.hpp"

EntityManager::EntityManager(RefPtr<Player> p) {
    player = p;
    entities.clear();

    slimeTex = Rendering::TextureManager::get().load_texture("./assets/enemies/slime.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);

    for(int i = 0; i < 4; i++){
        slimeSprites[i] = create_refptr<Graphics::G2D::Sprite>(slimeTex, Rendering::Rectangle{ {0, 0}, {32, 32} }, Rendering::Rectangle{ {0.0f + 0.25f * i, 0.0f}, {0.25f, 1.0f} });
    }

    ecount = 0;
}
EntityManager::~EntityManager() {
    entities.clear();
}


auto EntityManager::create_random_slime(glm::vec3 pos) -> void {
    srand(time(NULL));
    u8 r = rand() % 4;

    entities.emplace(
        ecount,
        create_refptr<Slime>(slimeSprites[r])
    );

    entities[ecount]->atk += r * 3;
    entities[ecount]->def += r * 2;
    entities[ecount]->pos = pos;

    ecount++;
}

auto EntityManager::update(World* wrld, double dt) -> void {
    for(auto& [id, e] : entities) {
        Enemy* enemy = reinterpret_cast<Enemy*>(e.get());
        enemy->update_enemy(wrld, dt, player->pos);
    }
}

auto EntityManager::draw() -> void {
    for(auto& [id, e] : entities) {
        Enemy* enemy = reinterpret_cast<Enemy*>(e.get());
        enemy->draw(player->rot);
    }
}
