#include "world.hpp"
#include "noiseutil.hpp"

World::World() {
    terrain_texture = Rendering::TextureManager::get().load_texture("./assets/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
    tree_texture = Rendering::TextureManager::get().load_texture("./assets/trees.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);
    NoiseUtil::initialize();

    chunk = new Chunk(0, 0, terrain_texture, tree_texture);
    
    chunk->generate_tile_data();
    chunk->generate_mesh_data();
}

World::~World() {

}

auto World::generate() -> void {

}

auto World::update(double dt) -> void {
    chunk->update(dt);
}

auto World::draw(Player* p) -> void {
    chunk->draw(p->rot);
}