#pragma once
#include <Stardust-Celeste.hpp>
#include <Graphics/2D/AnimatedTilemap.hpp>
#include "chunk.hpp"
#include "player.hpp"
#include <map>

using namespace Stardust_Celeste;

const uint32_t MAP_SIDE_LENGTH = 1024;

class World final {
    public:
    World(RefPtr<Player> player);
    ~World();

    auto update(double dt) -> void;
    auto draw() -> void;

    private:
        auto update_chunks() -> void;

        u32 terrain_texture, tree_texture;
        std::map<u32, Chunk*> mapData;
        RefPtr<Player> player;
};