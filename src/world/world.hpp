#pragma once
#include <Stardust-Celeste.hpp>
#include <Graphics/2D/AnimatedTilemap.hpp>
#include "chunk.hpp"
#include "../player/player.hpp"
#include "../entity/entityman.hpp"
#include <map>

class Player;
class EntityManager;
using namespace Stardust_Celeste;

const uint32_t MAP_SIDE_LENGTH = 1024;


class World final {
    public:
    World(RefPtr<Player> player);
    ~World();

    auto update(double dt) -> void;
    auto draw() -> void;
    auto tick() -> void;

    auto get_tile(glm::ivec2 pos) -> uint8_t;
    auto get_tile2(glm::ivec2 pos) -> uint8_t;
    auto set_tile(glm::ivec2 pos, uint8_t type) -> void;
    auto set_tile2(glm::ivec2 pos, uint8_t type) -> void;

    ScopePtr<EntityManager> eman;
    
    private:    
    uint32_t ticks;
    uint32_t lightLevel;
    uint32_t lastLevel;
    auto update_chunks() -> void;
    u32 terrain_texture, tree_texture;
    std::map<u32, Chunk*> mapData;
    RefPtr<Player> player;
};