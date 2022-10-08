#include "world.hpp"

World::World() {
    terrain_texture = Rendering::TextureManager::get().load_texture("./assets/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
    tilemap = create_scopeptr<Graphics::G2D::AnimatedTilemap>(terrain_texture, glm::vec2(16, 16));
}

World::~World() {

}

auto World::generate() -> void {
    for (uint32_t y = 0; y < MAP_SIDE_LENGTH; y++) {
        for (uint32_t x = 0; x < MAP_SIDE_LENGTH; x++) {
            auto idx = x + y * MAP_SIDE_LENGTH;
            tiles[idx] = 1;
        }
    }

    for (uint32_t y = 0; y < 64; y++) {
        for (uint32_t x = 0; x < 64; x++) {
            Graphics::G2D::AnimatedTile tile;
            tile.bounds = Rendering::Rectangle{ {x * TILE_SIZE, y * TILE_SIZE} , {TILE_SIZE, TILE_SIZE}};
            tile.color.color = 0xFFFFFFFF;
            tile.layer = 0;
            tile.index = tiles[x + y * MAP_SIDE_LENGTH];
            tile.start_idx = 0 + 16;
            tile.final_idx = 4 + 16;
            tilemap->add_tile(tile);
        }
    }

    tilemap->generate_map();
}

auto World::update(double dt) -> void {
    tilemap->update(dt);
}

auto World::draw() -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_rotate({-90, 0, -90});
    tilemap->draw();
}