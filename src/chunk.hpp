#pragma once
#include <Graphics/2D/AnimatedTilemap.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Stardust-Celeste.hpp>

using namespace Stardust_Celeste;
using namespace Graphics::G2D;

class Chunk {
  public:
    Chunk(int cx, int cy, u32 terrain, u32 tree);

    auto update(float dt) -> void;
    auto draw(float rot) -> void;


    auto generate_tile_data() -> void;
    auto generate_mesh_data() -> void;

  private:
    int cX, cY;
    uint8_t tiles[256];
    uint8_t biome[256];
    uint8_t layer2[256];
    ScopePtr<AnimatedTilemap> tmap;
    ScopePtr<Sprite> trees[4];
    ScopePtr<Sprite> flora[4];
};