#pragma once
#include <Stardust-Celeste.hpp>
#include <Graphics/2D/Tilemap.hpp>

using namespace Stardust_Celeste;

const uint32_t MAP_SIDE_LENGTH = 1024;
const uint32_t TILE_SIZE = 32;

class World final {
    public:
    World();
    ~World();

    auto generate() -> void;

    auto update(double dt) -> void;
    auto draw() -> void;

    private:
        std::array<uint8_t, 1024 * 1024> tiles;
        ScopePtr<Graphics::G2D::Tilemap> tilemap;
        u32 terrain_texture;
};