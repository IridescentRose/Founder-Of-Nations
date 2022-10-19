#include "world.hpp"
#include "noiseutil.hpp"
#include <cstdio>
#include <filesystem>

auto World::load(FILE* fptr) {
    if (fptr == NULL)
        return;
    
    fread(&NoiseUtil::seed, sizeof(NoiseUtil::seed), 1, fptr);
    NoiseUtil::initialize(NoiseUtil::seed);

    //Player Pos
    fread(&player->pos, sizeof(player->pos), 1, fptr);

    //Player ATK
    fread(&player->base_atk, sizeof(player->base_atk), 1, fptr);
    fread(&player->atk, sizeof(player->atk), 1, fptr);

    //Player DEF
    fread(&player->base_def, sizeof(player->base_def), 1, fptr);
    fread(&player->def, sizeof(player->def), 1, fptr);

    //Player ENERGY
    fread(&player->base_energy, sizeof(player->base_energy), 1, fptr);
    fread(&player->energy, sizeof(player->energy), 1, fptr);

    //MISC
    fread(&player->regen, sizeof(player->regen), 1, fptr);
    fread(&player->next_xp, sizeof(player->next_xp), 1, fptr);
    fread(&player->xp, sizeof(player->xp), 1, fptr);
    fread(&player->xpval, sizeof(player->xpval), 1, fptr);
    fread(&player->level, sizeof(player->level), 1, fptr);

    // World Time
    fread(&ticks, sizeof(ticks), 1, fptr);

    // Player Inventory
    for (int i = 0; i < 24; i++) {
        Slot slt;
        fread(&slt, sizeof(slt), 1, fptr);
        player->inventory->set_slot(i, slt);
    }

    fclose(fptr);
}

auto World::save(FILE* fptr) {
    if (fptr == NULL)
        return;

    //World Seed
    fwrite(&NoiseUtil::seed, sizeof(NoiseUtil::seed), 1, fptr);

    //Player Pos
    fwrite(&player->pos, sizeof(player->pos), 1, fptr);

    //Player ATK
    fwrite(&player->base_atk, sizeof(player->base_atk), 1, fptr);
    fwrite(&player->atk, sizeof(player->atk), 1, fptr);

    //Player DEF
    fwrite(&player->base_def, sizeof(player->base_def), 1, fptr);
    fwrite(&player->def, sizeof(player->def), 1, fptr);

    //Player ENERGY
    fwrite(&player->base_energy, sizeof(player->base_energy), 1, fptr);
    fwrite(&player->energy, sizeof(player->energy), 1, fptr);

    //MISC
    fwrite(&player->regen, sizeof(player->regen), 1, fptr);
    fwrite(&player->next_xp, sizeof(player->next_xp), 1, fptr);
    fwrite(&player->xp, sizeof(player->xp), 1, fptr);
    fwrite(&player->xpval, sizeof(player->xpval), 1, fptr);
    fwrite(&player->level, sizeof(player->level), 1, fptr);
    
    // World Time
    fwrite(&ticks, sizeof(ticks), 1, fptr);

    // Player Inventory
    for (int i = 0; i < 24; i++) {
        Slot slt = player->inventory->get_slot(i);
        fwrite(&slt, sizeof(slt), 1, fptr);
    }

    fclose(fptr);

    for (auto& [id, chk] : mapData) {
        chk->save();
    }
}


auto World::save_game(std::any a) -> void {
    World* wrld = std::any_cast<World*>(a);
    SC_APP_INFO("SAVING GAME!");
    if (wrld != nullptr) 
        wrld->save(fopen("savedata.dat", "wb"));
}

World::World(RefPtr<Player> player) {
    this->player = player;
    terrain_texture = Rendering::TextureManager::get().load_texture("./assets/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
    tree_texture = Rendering::TextureManager::get().load_texture("./assets/trees.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true);

    eman = create_scopeptr<EntityManager>(player);

    lightLevel = 15;
    lastLevel = lightLevel;
    ticks = 0;

    FILE* fptr = fopen("savedata.dat", "rb");
    if (fptr == NULL) {
        NoiseUtil::initialize();
        update_chunks();

        fptr = fopen("savedata.dat", "wb");
        std::filesystem::create_directory("world");
        save(fptr);
    }
    else {
        load(fptr);
    }
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

#define CHUNK_X_RANGE_MIN -2
#define CHUNK_X_RANGE_MAX 2
#define CHUNK_Z_RANGE_MIN -2
#define CHUNK_Z_RANGE_MAX 2

auto World::update_chunks() -> void {
    auto pos = player->pos;
    auto ivec = glm::ivec2(pos.x / (float)CHUNK_SIZE_X, pos.z / (float)CHUNK_SIZE_Y);

    std::vector<glm::ivec2> needed;
    std::map<u32, Chunk*> existing_chunks;

    for (int x = CHUNK_X_RANGE_MIN; x <= CHUNK_X_RANGE_MAX; x++) {
        for (int z = CHUNK_Z_RANGE_MIN; z <= CHUNK_Z_RANGE_MAX; z++) {
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
        val->save();
        delete val;
    }

    mapData.clear();
    mapData.merge(existing_chunks);

    for (auto& v : needed) {
        u32 id = (u16)(v.x) << 16 | (((u16)v.y) & 0x0FF);

        auto chunk = new Chunk(v.x, v.y, terrain_texture, tree_texture, lightLevel);
        if (!chunk->load()) {
            chunk->generate_tile_data();
        }
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

    for (auto& [id, chk] : mapData) {
        chk->tick(this);
    }
}