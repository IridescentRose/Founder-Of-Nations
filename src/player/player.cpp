#include "player.hpp"
#include "../world/tiles.hpp"
#include <gtx/rotate_vector.hpp>

Player::Player() : texture(0), invSelect(0) {
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
    pos = glm::vec3(1024 + 8, 0, 1024 + 8);

    hp = base_hp = 100;
    regen = 2;
    energy = base_energy = 100;
    xp = 10;

    atk = 8;
    base_atk = 8;

    inventory = create_refptr<Inventory>();

    inventory->set_slot(0, {Item::Sword, 1});
    inventory->set_slot(1, {Item::Pickaxe, 1});
    inventory->set_slot(2, {Item::Hoe, 1});
    inventory->set_slot(3, {Item::Axe, 1});
    inventory->set_slot(4, {Item::Shovel, 1});
    inventory->set_slot(5, {Item::Apple, 4});
}

Player::~Player() {

}

auto Player::update(World* wrld, double dt) -> void {
    if(triggerHit){
        triggerHit = false;
        //HIT
        wrld->eman->player_hit();

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
            test_vec += norm_vec * (float)i / 20.0f;
            auto t = (int)wrld->get_tile({ test_vec.x, test_vec.z });
            auto l = (int)wrld->get_tile2({ test_vec.x, test_vec.z });

            auto tool = inventory->get_slot(invSelect);
            if (tool.itemID == Item::Axe && l == Decorations::Tree) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                
                //TODO: Drop Resources
                break;
            }
            else if (tool.itemID == Item::Pickaxe && t == Tile::Stone) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);

                //TODO: Drop Resources
                break;
            }
            else if (tool.itemID == Item::Pickaxe && t == Tile::Stone_Coal) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);

                //TODO: Drop Resources
                break;
            }
            else if (tool.itemID == Item::Shovel && t == Tile::Sand) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);

                //TODO: Drop Resources
                break;
            }
            else if (tool.itemID == Item::Hoe && t == Tile::Grass) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland);
                //TODO: Drop Resources

                break;
            }
            else if (l == Decorations::Tallgrass) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                //TODO: Drop Resources

                break;
            }
            else if (l == Decorations::Flower) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                //TODO: Drop Resources

                break;
            }
            else if (l == Decorations::Bush) {
                wrld->set_tile2({ test_vec.x, test_vec.z }, Decorations::None);
                
                //TODO: Drop Resources
                break;
            }
            else if (tool.itemID == Item::Shovel && t == Tile::Grass) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Dirt);
                break;
            }
            else if (tool.itemID == Item::Hoe && t == Tile::Dirt) {
                wrld->set_tile({ test_vec.x, test_vec.z }, Tile::Farmland);
                break;
            }
        }
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
        if(item.itemID == Item::Sword) {
            atk = base_atk * 1.75;
        } else if (item.itemID == Item::Axe) {
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
                character->set_animation_range(38, 41);
            } else {
                character->set_animation_range(0, 3);
            }
        }
    } else {
        swingTimer -= dt;
        if (item.itemID == Item::Sword) {
            character->set_animation_range(43, 47);
        }
        else {
            character->set_animation_range(50, 54);
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
    
}

auto Player::draw() -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * PLAYER_SIZE, pos.y * PLAYER_SIZE, pos.z * PLAYER_SIZE });

    glDisable(GL_CULL_FACE);

    if(facing)
        Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    else 
        Rendering::RenderContext::get().matrix_rotate({ 0, rot + 180, 0 });
    
    Rendering::RenderContext::get().matrix_translate({ -50*0.75f, 0, 0 });
    character->draw();

    glEnable(GL_CULL_FACE);

    Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -30, 30);
    Rendering::RenderContext::get().set_mode_2D();
    Rendering::RenderContext::get().matrix_clear();
    ui->draw(inventory);
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
}