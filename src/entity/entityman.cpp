#include "entityman.hpp"
#include "../entity/enemy.hpp"
#include "../entity/slime.hpp"
#include "../world/tiles.hpp"

EntityManager::EntityManager(RefPtr<Player> p) {
    player = p;
    entities.clear();

    slimeTex = Rendering::TextureManager::get().load_texture("./assets/enemies/slime.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);

    for(int i = 0; i < 4; i++){
        slimeSprites[i] = create_refptr<Graphics::G2D::Sprite>(slimeTex, Rendering::Rectangle{ {0, 0}, {32, 32} }, Rendering::Rectangle{ {0.0f + 0.25f * i, 0.0f}, {0.25f, 1.0f} });
    }

    ecount = 0;
    mobCap = 6;
}
EntityManager::~EntityManager() {
    entities.clear();
}


auto EntityManager::create_random_slime(glm::vec3 pos) -> void {
    srand(time(NULL) + ecount);
    u8 r = rand() % 4;

    entities.emplace(
        ecount,
        create_refptr<Slime>(slimeSprites[r])
    );

    entities[ecount]->atk += r * 3;
    entities[ecount]->def += r;
    entities[ecount]->pos = pos;
    entities[ecount]->base_hp = 5 + r * 5;
    entities[ecount]->hp = entities[ecount]->base_hp;

    ecount++;
}



auto EntityManager::player_hit() -> void{
    std::vector<u32> ids;

    for(auto& [id, e] : entities) {
        auto diff = e->pos - player->pos;
        auto len = sqrtf(diff.x * diff.x + diff.z * diff.z);

        if(len < 1.75){
            e->take_damage(player.get());
            e->vel = diff * 5.0f;

            if(e->hp <= 0){
                ids.push_back(id);
            }
        }
    }

    for(auto& id : ids) {
        entities.erase(id);
    }
}


auto EntityManager::update(World* wrld, double dt) -> void {
    std::vector<u32> ids;

    for(auto& [id, e] : entities) {
        auto diff = e->pos - player->pos;
        auto len = sqrtf(diff.x * diff.x + diff.z * diff.z);

        if(len > 48) {
            ids.push_back(id);
        } else if(e->hp == 0) {
            ids.push_back(id);
        } else {
            e->update_enemy(wrld, dt, player->pos);

            if(len < 1) {
                player->take_damage(e.get());
            }
        }
    }

    for(auto& id : ids) {
        entities.erase(id);
    }
}

auto EntityManager::draw() -> void {
    for(auto& [id, e] : entities) {
        e->draw(player->rot);
    }
}

auto EntityManager::tick(World* wrld) -> void {
    if(mobCap > 0){

        for(int attempts = 30; attempts > 0; attempts--) {
            int x = rand() % 12;
            x -= 6;
            int z = rand() % 12;
            z -= 6;


            auto nPos = player->pos;
            nPos.x += x * 7;
            nPos.y = 0;
            nPos.z += z * 7;

            auto t = wrld->get_tile({nPos.x, nPos.z});
            auto l = wrld->get_tile2({nPos.x, nPos.z});

            if(t != Tile::Water_Heavy && t != Tile::Water_Light && t != Tile::Stone && t != Tile::Stone_Coal && l != 1) {
                create_random_slime(nPos);
                mobCap--;
                break;
            }
        }

    }
    for(auto& [id, e] : entities) {
        e->tick();
    }
}
