#pragma once
#include <Stardust-Celeste.hpp>
#include <Graphics/2D/AnimatedTilemap.hpp>
#include "chunk.hpp"
#include "player.hpp"

using namespace Stardust_Celeste;

const uint32_t MAP_SIDE_LENGTH = 1024;

class World final {
    public:
    World();
    ~World();

    auto generate() -> void;

    auto update(double dt) -> void;
    auto draw(Player* p) -> void;

    private:
        u32 terrain_texture, tree_texture;
        Chunk* chunk;
};