#include "entity.hpp"
#include "../world/tiles.hpp"
#include "../world/world.hpp"
#include "../sfxman.hpp"

auto test(glm::ivec2 pos, World* wrld) -> bool {
    auto t = wrld->get_tile(pos);
    auto l = wrld->get_tile2(pos);
    return t == Tile::Stone || t == Tile::Stone_Coal || l == 1;
}

auto Entity::tick() -> void {
    if(iframes > 0)
        iframes--;
    else {
        hp += regen;
        if(hp > base_hp)
            hp = base_hp;
    }
}

auto Entity::take_damage(Entity* e) -> void {
    if(e == nullptr)
        return;
    
    if(iframes != 0 || immortal)
        return;

    int nhp = hp;

    int dmg = e->atk - def;
    if(dmg > 0) {
        nhp -= dmg;
        iframes = 1;

        if (nhp < 0)
            hp = 0;
        else
            hp = nhp;

        if (hp == 0) {
            e->xp += xpval;
            SFXManager::get().play(SFX_TYPE_KILL);
        } else {
            SFXManager::get().play(SFX_TYPE_HIT);
        }
    }
}

auto Entity::update(World* wrld, double dt) -> void {
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

    auto t = wrld->get_tile({ (int)pos.x, (int)pos.z });
    in_water = t == Tile::Water_Heavy || t == Tile::Water_Light;

    if (t == Tile::Water_Heavy) {
        vel *= 0.8f;
    } else if (t == Tile::Water_Light) {
        vel *= 0.9f;
    }

    pos += vel * (float)dt;


    vel *= 0.9f;
    acc = glm::vec3(0, 0, 0);
}
