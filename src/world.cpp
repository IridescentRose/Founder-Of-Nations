#include "world.hpp"

World::World() {
    terrain_texture = Rendering::TextureManager::get().load_texture("./assets/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
    tilemap = create_scopeptr<Graphics::G2D::Tilemap>(terrain_texture, glm::vec2(16, 16));
}

World::~World() {

}

auto World::generate() -> void {
    for (uint32_t y = 0; y < MAP_SIDE_LENGTH; y++) {
        for (uint32_t x = 0; x < MAP_SIDE_LENGTH; x++) {
            tiles[x + y * MAP_SIDE_LENGTH] = 0;
        }
    }

    for (uint32_t y = 0; y < 256; y++) {
        for (uint32_t x = 0; x < 256; x++) {
            Graphics::G2D::Tile tile = { 
                { {x * TILE_SIZE, y * TILE_SIZE} , {TILE_SIZE, TILE_SIZE}},
                {255, 255, 255, 255}, 
                tiles[x + y * MAP_SIDE_LENGTH], 
                0.0f 
            };

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
    Rendering::RenderContext::get().matrix_rotate({-90, 0, 0});
    tilemap->draw();
}