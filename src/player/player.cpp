#include "player.hpp"
#include "../world/tiles.hpp"
#include <gtx/rotate_vector.hpp>
#include "../sfxman.hpp"
#include "../musman.hpp"
#include <ctime>

Player::Player() : texture(0), invSelect(0), inInventory(false) {
	texture = Rendering::TextureManager::get().load_texture("./assets/charsheet.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);

    camera = create_scopeptr<Rendering::Camera>(
        glm::vec3(0, 0, 0),
        glm::vec3(degtorad(30.0f), 0, 0),
        degtorad(45.0f),
        16.0f / 9.0f,
        0.3f,
        128.0f * PLAYER_SIZE
    );

    Rendering::RenderContext::get().set_mode_3D();

    character = create_scopeptr<Graphics::G2D::AnimatedSprite>(
        texture,
        Rendering::Rectangle{ {0, 0}, {50 * 1.5f, 37 * 1.5f} },
        glm::vec2(7, 11)
    );
    character->set_animation_range(0, 3);

    ui = create_scopeptr<UI>(*this);

    rot = 0.0f;
    pos = glm::vec3(1032, 0, 1032);

    hp = base_hp = 100;
    regen = 2;
    energy = base_energy = 100;
    xp = 10;

    atk = 8;
    base_atk = 8;

    inventory = create_refptr<Inventory>();

    inventory->set_slot(0, {Item::Sword, 1});
    inventory->set_slot(1, {Item::Bow, 1});
    inventory->set_slot(2, {Item::Pickaxe, 1});
    inventory->set_slot(3, {Item::Hoe, 1});
    inventory->set_slot(4, {Item::Axe, 1});
    inventory->set_slot(5, {Item::Shovel, 1});

    swing = false;

    triggerUse = false;
    dead = false;
    deathTimer = 5.0f;
    MUSManager::get().play(rand() % 4);
}

Player::~Player() {

}

auto Player::update(World* wrld, double dt) -> void {
    if(triggerHit){
        triggerHit = false;
        //HIT
        {
            auto tool = inventory->get_slot(invSelect);
            if (tool.itemID != Item::Bow) {
                wrld->eman->player_hit();
                SFXManager::get().play(SFX_TYPE_SWING);
            } 
            else {
                glm::vec3 norm_vec(-1.0f, 0.0f, 0.0f);
                if (facing) {
                    norm_vec = glm::rotateY(norm_vec, degtorad(rot + 180.0f));
                }
                else {
                    norm_vec = glm::rotateY(norm_vec, degtorad(rot));
                }
                norm_vec *= 8;
                wrld->eman->player_arrow(facing, pos, norm_vec + vel, atk * 2);
                SFXManager::get().play(SFX_TYPE_SHOOT);
            }
        }

        //CHECK BLOCK HIT & TOOL
        glm::vec3 norm_vec(-1.0f, 0.0f, 0.0f);

        if (facing) {
            norm_vec = glm::rotateY(norm_vec, degtorad(rot + 180.0f));
        }
        else {
            norm_vec = glm::rotateY(norm_vec, degtorad(rot));
        }

        for (int i = 0; i < 50; i++) {
            auto test_vec = pos;
            test_vec += norm_vec * (float)i / 25.0f;
            auto t = (int)wrld->get_tile({ test_vec.x, test_vec.z });
            auto l = (int)wrld->get_tile2({ test_vec.x, test_vec.z });

            auto tool = inventory->get_slot(invSelect);
            if (tool.itemID == Item::Bow)
                break;

            if (tool.itemID == Item::Axe && l == Decorations::Tree) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                wrld->eman->create_drop(test_vec, Item::Log, 2, 3);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            if (tool.itemID == Item::Axe && t == Tile::Wood) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                wrld->eman->create_drop(test_vec, Item::Log, 1, 0);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Pickaxe && t == Tile::Stone) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                wrld->eman->create_drop(test_vec, Item::Stone, 2, 3);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Pickaxe && t == Tile::Stone_Coal) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                wrld->eman->create_drop(test_vec, Item::Coal, 1, 1);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Pickaxe && t == Tile::Cobblestone) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                wrld->eman->create_drop(test_vec, Item::Stone, 1, 0);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Shovel && t == Tile::Sand) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                wrld->eman->create_drop(test_vec, Item::Sand, 1, 2);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Hoe && t == Tile::Grass) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland);
                wrld->eman->create_drop(test_vec, Item::Seed, 1, 0);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (l == Decorations::Tallgrass) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                wrld->eman->create_drop(test_vec, Item::Seed, 1, 0);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (l == Decorations::Flower) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                wrld->eman->create_drop(test_vec, Item::Seed, 1, 0);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (l == Decorations::Bush) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                wrld->eman->create_drop(test_vec, Item::Berries, 1, 1);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Shovel && t == Tile::Grass) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Hoe && t == Tile::Dirt) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Hoe && (t == Tile::Farmland1 || t == Tile::Farmland2 || t == Tile::Farmland3)) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland);
                wrld->eman->create_drop(test_vec, Item::Seed, 1, 0);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
            else if (tool.itemID == Item::Hoe && t == Tile::Farmland4) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland);
                wrld->eman->create_drop(test_vec, Item::Seed, 1, 0);
                wrld->eman->create_drop(test_vec, Item::Wheat, 1, 3);
                energy += 5;
                SFXManager::get().play(SFX_TYPE_HIT);
                break;
            }
        }
    }
    if (triggerUse) {
        triggerUse = false;

        Slot& item = inventory->get_slot(invSelect);
        if (item.itemID == Item::Berries) {
            hp += 10;
            item.count -= 1;
        }
        else if (item.itemID == Item::Bread) {
            hp += 20;
            item.count -= 1;
        }
        else if (item.itemID == Item::GoldenApple) {
            hp += 100;
            item.count -= 1;
        }
        else {
            //Try place
            glm::vec3 norm_vec(-1.0f, 0.0f, 0.0f);

            if (facing) {
                norm_vec = glm::rotateY(norm_vec, degtorad(rot + 180.0f));
            }
            else {
                norm_vec = glm::rotateY(norm_vec, degtorad(rot));
            }

            for (int i = 0; i < 50; i++) {
                auto test_vec = pos;
                test_vec += norm_vec * (float)i / 25.0f;

                if ((int)test_vec.x == (int)pos.x && (int)test_vec.z == (int)pos.z)
                    continue;

                auto t = (int)wrld->get_tile({ test_vec.x, test_vec.z });
                auto l = (int)wrld->get_tile2({ test_vec.x, test_vec.z });

                if (l != 0)
                    continue;

                if (item.itemID == Item::Seed && t == Tile::Farmland) {
                    wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland1);
                    item.count -= 1;
                    break;
                }
                else if (item.itemID == Item::Log && (t == Tile::Sand || t == Tile::Water_Light || t == Tile::Dirt || t == Tile::Grass)) {
                    wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Wood);
                    item.count -= 1;
                    break;
                }
                else if (item.itemID == Item::Stone && (t == Tile::Sand || t == Tile::Water_Light || t == Tile::Dirt || t == Tile::Grass)) {
                    wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Cobblestone);
                    item.count -= 1;
                    break;
                }
                else if (item.itemID == Item::Sand && (t == Tile::Water_Heavy)) {
                    wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Water_Light);
                    item.count -= 1;
                    break;
                }
            }

        }

        if (item.count == 0)
            item.itemID = Item::None;
    }
    character->update(dt);

    float len = sqrtf(acc.x * acc.x + acc.z * acc.z);

    if(len > 0) {
        acc /= len;
        acc *= PLAYER_ACCELERATION;
    }

    ui->slot_sel = invSelect;

    Slot item = inventory->get_slot(invSelect);
    if(item.count > 0){
        if (item.itemID == Item::Sword) {
            atk = base_atk * 1.75;
        }
        else if (item.itemID == Item::Bow) {
            atk = base_atk * 1.75;
        }
        else if (item.itemID == Item::Axe) {
            atk = base_atk * 1.5;
        } else {
            atk = base_atk;
        }
    } else {
        atk = base_atk;
    }

    if(!swing){

        character->ticksPerSec = 4.0f;
        if(acc.x != 0 || acc.z != 0){
            character->set_animation_range(9, 14);
        }else {
            if(item.itemID == Item::Sword) {
                character->set_animation_range(31, 34);
            } else {
                character->set_animation_range(0, 3);
            }
        }
    } 
    else {
        swingTimer -= dt;
        if (item.itemID == Item::Sword) {
            character->set_animation_range(43, 47);
        }
        else if(item.itemID == Item::Bow) {
            character->set_animation_range(49, 57);
        } else {
            character->set_animation_range(58, 62);
        }
        character->ticksPerSec = 8.0f;

        if(swingTimer <= 0) {
            swing = false;
        }
    }

    Entity::update(wrld, dt);

    camera->pos = pos * PLAYER_SIZE;
    camera->pos.y += 4.2f * PLAYER_SIZE;
    camera->pos.x += sinf(degtorad(rot)) * 6.0f * PLAYER_SIZE;
    camera->pos.z += cosf(degtorad(rot)) * 6.0f * PLAYER_SIZE;
    camera->rot.y = degtorad(-rot);
    Rendering::RenderContext::get().set_mode_3D();
    camera->update();

    time = wrld->get_tick_time();

    deathTimer += dt;
    if (hp <= 0 && deathTimer > 5.0f && !dead) {
        dead = true;
        deathTimer = 0.0f;
        regen = 0;
        MUSManager::get().play(MUS_TYPE_DEATH);
    }
    else if (deathTimer > 5.0f && dead) {
        //Respawn
        regen = 2;
        dead = false;
        xp = 0;
        xpval = 0;
        next_xp = 100;
        level = 0;
        hp = base_hp;

        pos.x += rand() % 17 - 9;
        pos.z += rand() % 17 - 9;
        MUSManager::get().play(rand() % 4);
    }
}

auto Player::draw() -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * PLAYER_SIZE, pos.y * PLAYER_SIZE, pos.z * PLAYER_SIZE });


#ifndef PSP
    glDisable(GL_CULL_FACE);
#else 
    sceGuDisable(GU_CULL_FACE);
#endif

    if(facing)
        Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    else 
        Rendering::RenderContext::get().matrix_rotate({ 0, rot + 180, 0 });
    
    Rendering::RenderContext::get().matrix_translate({ -50*0.75f, 0, 0 });
    character->draw();

    Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -30, 30);
    Rendering::RenderContext::get().set_mode_2D();
    Rendering::RenderContext::get().matrix_clear();
    
#ifndef PSP
    glEnable(GL_CULL_FACE);
#else 
    sceGuEnable(GU_CULL_FACE);
#endif

    ui->draw(inventory, inInventory, time, dead);
}


auto Player::tick() -> void {
    energy += 2; //TODO: ENERGY REGEN

    if(in_water){
        if(energy > 3)
            energy -= 3;
        else 
            energy = 0;
    }

    if(energy <= 0){
        energy = 0;
        hp -= 5;
    }

    if(energy > base_energy)
        energy = base_energy;
    Entity::tick();

    if (xp > next_xp) {
        level++;
        xp -= next_xp;
        next_xp = next_xp * 1.02f;
    }
}