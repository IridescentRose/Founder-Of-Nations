#include "entityman.hpp"
#include "beholder.hpp"
#include "drop.hpp"
#include "enemy.hpp"
#include "slime.hpp"
#include "spider.hpp"
#include "arrow.hpp"
#include "../world/tiles.hpp"

EntityManager::EntityManager(RefPtr<Player> p) {
    player = p;
    entities.clear();
    drops.clear();

    beholderTex = Rendering::TextureManager::get().load_texture("./assets/enemies/beholder.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);
    slimeTex = Rendering::TextureManager::get().load_texture("./assets/enemies/slime.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);
    spiderTex = Rendering::TextureManager::get().load_texture("./assets/enemies/spider.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);
    arrowTex = Rendering::TextureManager::get().load_texture("./assets/arrow.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);
    itemID = Rendering::TextureManager::get().load_texture("./assets/items.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);

    arrowSprite = create_refptr<Graphics::G2D::Sprite>(arrowTex, Rendering::Rectangle{ {0,0}, {32, 32}});

    for(int i = 0; i < 4; i++){
        beholderSprites[i] = create_refptr<Graphics::G2D::Sprite>(beholderTex, Rendering::Rectangle{ {0, 0}, {64, 64} }, Rendering::Rectangle{ {0.0f + 0.25f * i, 0.0f}, {0.25f, 1.0f} });
        slimeSprites[i] = create_refptr<Graphics::G2D::Sprite>(slimeTex, Rendering::Rectangle{ {0, 0}, {32, 32} }, Rendering::Rectangle{ {0.0f + 0.25f * i, 0.0f}, {0.25f, 1.0f} });
        spiderSprites[i] = create_refptr<Graphics::G2D::Sprite>(spiderTex, Rendering::Rectangle{ {0, 0}, {32, 32} }, Rendering::Rectangle{ {0.0f + 0.25f * i, 0.0f}, {0.25f, 1.0f} });
    }

	for(int i = 0; i < 64; i++) {
		items[i] = create_scopeptr<Graphics::G2D::Sprite>(itemID,
		Rendering::Rectangle{ {0, 0}, {16, 16} },
		Rendering::Rectangle{ {0.125f * static_cast<float>(i % 8), 0.125f * static_cast<float>((i / 8) + 1) }, {16.0f / 128.0f, -16.0f / 128.0f} });
	}

    acount = 0;
    ecount = 0;
    icount = 0;
    mobCap = totalCap = 5;
}
EntityManager::~EntityManager() {
    entities.clear();
    drops.clear();
}


auto EntityManager::player_arrow(bool facing, glm::vec3 pos, glm::vec3 vel, uint32_t arrowATK) -> void {
    arrows.emplace(
        acount,
        create_refptr<ArrowEntity>(arrowSprite, vel)
    );
    arrows[acount]->pos = pos;
    arrows[acount]->vel = vel;
    arrows[acount]->atk = arrowATK;
    arrows[acount]->facing = facing;

    acount++;
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
    entities[ecount]->xpval = r * 2 + 3;

    ecount++;
    mobCap--;
}

auto EntityManager::create_random_beholder(glm::vec3 pos) -> void {
    srand(time(NULL) + ecount);
    u8 r = rand() % 4;

    entities.emplace(
        ecount,
        create_refptr<Beholder>(beholderSprites[r])
    );

    entities[ecount]->atk += r * 5;
    entities[ecount]->def += r * 2;
    entities[ecount]->pos = pos;
    entities[ecount]->base_hp = 10 + r * 6;
    entities[ecount]->hp = entities[ecount]->base_hp;
    entities[ecount]->xpval = r * 5 + 3;

    ecount++;
    mobCap--;
}

auto EntityManager::create_random_spider(glm::vec3 pos) -> void {
    for (int i = 0; i < 3 && mobCap > 0; i++) {
        srand(time(NULL) + ecount);
        u8 r = rand() % 4;

        entities.emplace(
            ecount,
            create_refptr<Spider>(spiderSprites[r])
        );

        entities[ecount]->atk += r;
        entities[ecount]->def += r * 0.5f;
        entities[ecount]->pos = pos;
        entities[ecount]->base_hp = 3 + r * 4;
        entities[ecount]->acceleration_speed += r * 4.0f;
        entities[ecount]->hp = entities[ecount]->base_hp;
        entities[ecount]->xpval = r * 1 + 2;

        ecount++;
        mobCap--;
    }
}

auto EntityManager::create_drop(glm::vec3 pos, uint8_t item, uint16_t count, uint16_t random) -> void {
    if (item == 0)
        return;

    srand(time(NULL) + icount);
    uint16_t r = rand() % random;
    uint16_t cnt = count + r;

    if (cnt > 0) {
        drops.emplace(
            icount,
            create_refptr<ItemDrop>(items[item - 1])
        );
        drops[icount]->itemData = Slot{ item, cnt };
        drops[icount]->pos = pos;
        icount++;
    }
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
        mobCap++;
        if (mobCap > totalCap) {
            mobCap = totalCap;
        }
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
        mobCap++;
        if (mobCap > totalCap) {
            mobCap = totalCap;
        }
    }

    ids.clear();

    for(auto& [id, e] : drops) {
        auto diff = e->pos - player->pos;
        auto len = sqrtf(diff.x * diff.x + diff.z * diff.z);

        e->update_drop(wrld, dt, player->pos);

        if(len < 0.5f) {
            player->inventory->add_item(e->itemData);
            ids.push_back(id);
        }
    }

    for(auto& id : ids) {
        drops.erase(id);
    }

    ids.clear();

    for (auto& [id, a] : arrows) {
        a->update_arrow(wrld, dt);
        for (auto& [id2, e] : entities) {
            auto diff = a->pos - e->pos;
            auto len = sqrtf(diff.x * diff.x + diff.z * diff.z);

            if (len < 0.5f) {
                e->take_damage(a.get());
                ids.push_back(id);
                break;
            }

            if (a->timer > 4.0f) {
                ids.push_back(id);
                break;
            }
        }
    }

    for (auto& id : ids) {
        arrows.erase(id);
    }
}

auto EntityManager::draw() -> void {
    for(auto& [id, e] : entities) {
        e->draw(player->rot);
    }

    for (auto& [id, e] : drops) {
        e->draw(player->rot);
    }

    for (auto& [id, e] : arrows) {
        e->draw(player->rot);
    }
}

auto EntityManager::tick(World* wrld) -> void {
    if (wrld->get_light_level() > 8) {
        if (totalCap == 12) {
            totalCap = 5;
            mobCap = totalCap;
        }
    }
    else {
        if (totalCap == 5) {
            totalCap = 12;
            mobCap = totalCap;
        }
    }

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
                int r = rand() % 2;
                
                if(wrld->get_light_level() > 8) {
                    create_random_slime(nPos);
                } else {
                    if(r == 0) 
                        create_random_spider(nPos);
                    else
                        create_random_beholder(nPos);
                }
                break;
            }
        }

    }
    for(auto& [id, e] : entities) {
        e->tick();
    }
}
