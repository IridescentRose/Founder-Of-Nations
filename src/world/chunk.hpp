#pragma once
#include <Graphics/2D/AnimatedTilemap.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Stardust-Celeste.hpp>

using namespace Stardust_Celeste;
using namespace Graphics;

const uint32_t CHUNK_SIZE_X = 16;
const uint32_t CHUNK_SIZE_Y = 16;
const uint32_t TILE_SIZE = 32;

class Chunk final {
  public:
    Chunk(int cx, int cy, u32 terrain, u32 tree, u32 light);

    auto update(float dt) -> void;
    auto update_lighting(uint32_t level) -> void;
    auto draw(float rot) -> void;

    auto generate_tile_data() -> void;
    auto generate_mesh_data() -> void;

    uint8_t tiles[256];
    uint8_t layer2[256];
    uint8_t biome[256];
  private:
    uint32_t lightLevel;
    int cX, cY;
    ScopePtr<G2D::AnimatedTilemap> tmap;
    ScopePtr<G2D::Sprite> flora[16];
};