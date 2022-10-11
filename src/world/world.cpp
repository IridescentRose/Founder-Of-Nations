#include "world.hpp"
#include "noiseutil.hpp"

World::World(RefPtr<Player> player) {
    this->player = player;
    terrain_texture = Rendering::TextureManager::get().load_texture("./assets/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
    tree_texture = Rendering::TextureManager::get().load_texture("./assets/trees.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);

    eman = create_scopeptr<EntityManager>(player);

    NoiseUtil::initialize();

    update_chunks();    

    lightLevel = 15;
    lastLevel = lightLevel;
    ticks = 0;
}

World::~World() {

}

auto World::get_tile(glm::ivec2 pos)-> uint8_t {
    u16 cX = pos.x / CHUNK_SIZE_X;
    u16 cZ = pos.y / CHUNK_SIZE_Y;
    u32 id = ((u16)(cX) << 16) | ((u16)(cZ) & 0x0FF);

    if (mapData.find(id) != mapData.end()) {
        auto chunk = mapData[id];
        u16 localX = pos.x - cX * CHUNK_SIZE_X;
        u16 localZ = pos.y - cZ * CHUNK_SIZE_Y;

        u32 idx = localX + localZ * CHUNK_SIZE_X;
        return chunk->tiles[idx];
    }

    return 255;
}
auto World::get_tile2(glm::ivec2 pos)-> uint8_t {
    u16 cX = pos.x / CHUNK_SIZE_X;
    u16 cZ = pos.y / CHUNK_SIZE_Y;
    u32 id = ((u16)(cX) << 16) | ((u16)(cZ) & 0x0FF);

    if (mapData.find(id) != mapData.end()) {
        auto chunk = mapData[id];
        u16 localX = pos.x - cX * CHUNK_SIZE_X;
        u16 localZ = pos.y - cZ * CHUNK_SIZE_Y;

        u32 idx = localX + localZ * CHUNK_SIZE_X;
        return chunk->layer2[idx];
    }

    return 255;
}

auto World::set_tile(glm::ivec2 pos, uint8_t type) -> void {
    u16 cX = pos.x / CHUNK_SIZE_X;
    u16 cZ = pos.y / CHUNK_SIZE_Y;
    u32 id = ((u16)(cX) << 16) | ((u16)(cZ) & 0x0FF);

    if (mapData.find(id) != mapData.end()) {
        auto chunk = mapData[id];
        u16 localX = pos.x - cX * CHUNK_SIZE_X;
        u16 localZ = pos.y - cZ * CHUNK_SIZE_Y;

        u32 idx = localX + localZ * CHUNK_SIZE_X;
        chunk->tiles[idx] = type;
        chunk->generate_mesh_data();
    }
}

auto World::set_tile2(glm::ivec2 pos, uint8_t type) -> void {
    u16 cX = pos.x / CHUNK_SIZE_X;
    u16 cZ = pos.y / CHUNK_SIZE_Y;
    u32 id = ((u16)(cX) << 16) | ((u16)(cZ) & 0x0FF);

    if (mapData.find(id) != mapData.end()) {
        auto chunk = mapData[id];
        u16 localX = pos.x - cX * CHUNK_SIZE_X;
        u16 localZ = pos.y - cZ * CHUNK_SIZE_Y;

        u32 idx = localX + localZ * CHUNK_SIZE_X;
        chunk->layer2[idx] = type;
    }
}

auto World::update_chunks() -> void {
    auto pos = player->pos;
    auto ivec = glm::ivec2(pos.x / (float)CHUNK_SIZE_X, pos.z / (float)CHUNK_SIZE_Y);

    std::vector<glm::ivec2> needed;
    std::map<u32, Chunk*> existing_chunks;

    for (int x = -2; x <= 2; x++) {
        for (int z = -2; z <= 2; z++) {
            u32 id = ((u16)(ivec.x + x) << 16) | ((u16)(ivec.y + z) & 0x0FF);

            if (mapData.find(id) != mapData.end()) {
                existing_chunks.emplace(id, mapData[id]);
                mapData.erase(id);
            }
            else {
                needed.push_back(ivec + glm::ivec2(x, z));
            }
        }
    }

    for (auto& [key, val] : mapData) {
        delete val;
    }

    mapData.clear();
    mapData.merge(existing_chunks);

    for (auto& v : needed) {
        u32 id = (u16)(v.x) << 16 | (((u16)v.y) & 0x0FF);

        auto chunk = new Chunk(v.x, v.y, terrain_texture, tree_texture, lightLevel);
        chunk->generate_tile_data();
        chunk->generate_mesh_data();
        mapData.emplace(id, chunk);
    }
}

auto World::update(double dt) -> void {
    update_chunks();

    for (auto& [key, val] : mapData) {
        val->update(dt);
    }

    eman->update(this, dt);
}

auto World::draw() -> void {
    for (auto& [key, val] : mapData) {
        val->draw(player->rot);
    }

    eman->draw();
}

auto World::tick() -> void {
    ticks += 2;

    //2400 is full cycle
    auto daytime = ticks % 2400;

    if (daytime >= 0 && daytime < 200) {
        //Morning, getting brighter
        lightLevel = 11 + daytime / 50;
    }
    else if (daytime >= 200 && daytime < 1000) {
        //Day, constant brightness
        lightLevel = 15;
    }
    else if (daytime >= 1000 && daytime < 1400) {
        //Evening/Twilight, getting darker
        lightLevel = 15 - (daytime - 1000) / 50;
    }
    else if (daytime >= 1400 && daytime < 2200) {
        //Night, constant brightness
        lightLevel = 7;
    } 
    else if (daytime >= 2200 && daytime < 2400) {
        //Morning, getting brighter
        lightLevel = 7 + (daytime - 2200) / 50;
    }
    else {
        //Invalid -- day brightness
        lightLevel = 15;
    }

    if (lastLevel != lightLevel) {
        lastLevel = lightLevel;
        //Update Chunk Lighting;

        for (auto& [id, chk] : mapData) {
            chk->update_lighting(lightLevel);
        }
    }


    player->tick();
    eman->tick(this);
}