#include "chunk.hpp"
#include "generator.hpp"
#include "world.hpp"

Chunk::Chunk(int cx, int cy, u32 terrain, u32 tree, u32 level) : cX(cx), cY(cy) {
    tmap = create_scopeptr<G2D::AnimatedTilemap>(
        terrain,
        glm::vec2{16, 16});

    for(int i = 0; i < 16; i++){
        flora[i] = create_scopeptr<G2D::Sprite>(
            tree,
            Rendering::Rectangle{ {0, 0}, {40, 60}},
            Rendering::Rectangle{ {0.25f * (i % 4), 0.75f - (i / 4) * 0.25f}, {0.25, 0.25}}
        );
    }

    tmap->tickPerSec = 2.0f;

    for (auto i = 0; i < 256; i++) {
        tiles[i] = 0;
        biome[i] = 0;
        layer2[i] = 0;
    }

    lightLevel = level;
}

auto Chunk::update(float dt) -> void {
    tmap->update(dt);
}
struct TileData {
    u8 start_frame;
    u8 frame_count;
};


auto Chunk::tick(World* wrld) -> void {
    // Random ticks
    for (int i = 0; i < 4; i++) {
        auto rx = rand() % 16;
        auto ry = rand() % 16;

        auto p = glm::vec2(rx + cX * 16, ry + cY * 16);
        auto t = wrld->get_tile(p);

        if (t == Tile::Dirt) {
            wrld->set_tile(p, Tile::Grass);
        }
        else if (t == Tile::Farmland) {
            wrld->set_tile(p, Tile::Dirt);
        }
        else if (t == Tile::Farmland1) {
            wrld->set_tile(p, Tile::Farmland2);
        }
        else if (t == Tile::Farmland2) {
            wrld->set_tile(p, Tile::Farmland3);
        }
        else if (t == Tile::Farmland3) {
            wrld->set_tile(p, Tile::Farmland4);
        }
    }

    // Controlled ticks
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            auto pos = glm::vec2(x + cX * 16, y + cY * 16);
            auto tile = wrld->get_tile(pos);

            if (tile == Tile::Dirt) {
                //Check for water around it
                glm::vec2 checks[4];
                for (int i = 0; i < 4; i++)
                    checks[i] = pos;
                
                checks[0].x += 1;
                checks[1].x -= 1;
                checks[2].y += 1;
                checks[3].y -= 1;

                for (int i = 0; i < 4; i++) {
                    auto t2 = wrld->get_tile(checks[i]);
                    // Found water, fill this tile, and leave
                    if (t2 == Tile::Water_Heavy || t2 == Tile::Water_Light) {
                        wrld->set_tile(pos, Tile::Water_Light);
                        break;
                    }
                }

                
            }
        }
    }
}

auto Chunk::save() -> void {
    u32 id = (cX << 16) | (cY & 0x0FF);
    auto filename = "world/" + std::to_string(id) + ".chk";
    FILE* fptr = fopen(filename.c_str(), "wb");

    if (fptr == NULL)
        return;

    fwrite(tiles, sizeof(uint8_t), 256, fptr);
    fwrite(layer2, sizeof(uint8_t), 256, fptr);
    fwrite(biome, sizeof(uint8_t), 256, fptr);
    
    fclose(fptr);
}

auto Chunk::load() -> bool {
    u32 id = (cX << 16) | (cY & 0x0FF);
    auto filename = "world/" + std::to_string(id) + ".chk";
    FILE* fptr = fopen(filename.c_str(), "rb");

    if (fptr == NULL)
        return false;


    fread(tiles, sizeof(uint8_t), 256, fptr);
    fread(layer2, sizeof(uint8_t), 256, fptr);
    fread(biome, sizeof(uint8_t), 256, fptr);

    fclose(fptr);

    return true;
}

// DATA MAP
auto get_tile_data(uint8_t id, uint8_t biome) -> TileData {
    switch (id) {
    case Tile::Water_Light:
        return {0, 4};
    case Tile::Water_Heavy:
        return {16, 20};
    case Tile::Sand:
        return {80, 1};
    case Tile::Dirt:
        return {4, 1};
    case Tile::Grass:
        return {static_cast<u8>(biome * 16 + 32), 1};
    case Tile::Grass_Tall:
        return {static_cast<u8>(biome * 16 + 32 + 3), 2};
    case Tile::Grass_Bush:
        return {static_cast<u8>(biome * 16 + 32 + 5), 2};
    case Tile::Grass_Flower:
        return {static_cast<u8>(biome * 16 + 32 + 1), 2};
    case Tile::Stone:
        return {20, 1};
    case Tile::Stone_Coal:
        return { 21, 1 };
    case Tile::Cave_Entrance:
        return {22, 1};
    case Tile::Cobblestone:
        return { 23, 1 };
    case Tile::Wood:
        return { 24, 1 };
    case Tile::Grass_Mushroom:
        return {static_cast<u8>(biome * 16 + 32 + 7), 2};

    case Tile::Cave_Dirt:
        return {96, 1};
    case Tile::Cave_Exit:
        return {97, 1};
    case Tile::Ore_Copper:
        return {99, 1};
    case Tile::Ore_Iron:
        return {98, 1};
    case Tile::Ore_Gold:
        return {100, 1};
    case Tile::Ore_Crystal:
        return {101, 1};

    case Tile::Farmland:
        return { 5, 1 };
    case Tile::Farmland1:
        return { 6, 1 };
    case Tile::Farmland2:
        return { 7, 1 };
    case Tile::Farmland3:
        return { 8, 1 };
    case Tile::Farmland4:
        return { 9, 1 };

    default:
        return {id, 1};
    }
}

auto create_tile(uint8_t id, uint8_t biome, glm::vec2 pos, uint32_t level) -> G2D::AnimatedTile {
    auto data = get_tile_data(id, biome);

    auto col = 16 * level + level;

    Rendering::Color color;
    color.rgba.r = col;
    color.rgba.g = col;
    color.rgba.b = col;
    color.rgba.a = 255;

    return Graphics::G2D::AnimatedTile {
        Rendering::Rectangle{glm::vec2{pos.x * TILE_SIZE, pos.y * TILE_SIZE},
                             glm::vec2{TILE_SIZE, TILE_SIZE}},
        color,
        data.start_frame,
        0.0f,
        data.start_frame,
        data.frame_count,
    };
}


auto Chunk::update_lighting(uint32_t level) -> void {
    lightLevel = level;

    generate_mesh_data();
}

auto Chunk::generate_tile_data() -> void {
    WorldGen::generate(cX, cY, tiles, biome, layer2);
}

auto Chunk::generate_mesh_data() -> void {
    tmap->clear_tiles();
    
    for (int x = 0; x < CHUNK_SIZE_X; x++)
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            auto idx = x + y * CHUNK_SIZE_X;
            tmap->add_tile(create_tile(tiles[idx], biome[idx], {x, y}, lightLevel));
        }


    auto col = 16 * lightLevel + lightLevel;

    Rendering::Color color;
    color.rgba.r = col;
    color.rgba.g = col;
    color.rgba.b = col;
    color.rgba.a = 255;
    for (int i = 0; i < 16; i++) {
        flora[i]->set_color(color);
    }

    tmap->generate_map();
}

auto Chunk::draw(float rot) -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate(
        glm::vec3(
            cX * TILE_SIZE * CHUNK_SIZE_X, 
            0, 
            cY * TILE_SIZE * CHUNK_SIZE_Y)
    );
#ifndef PSP
    glFrontFace(GL_CW);
#endif
    Rendering::RenderContext::get().matrix_rotate({90, 0, 0});
    tmap->draw();

#ifndef PSP
    glFrontFace(GL_CCW);
#endif
    
    for(uint32_t y = 0; y < CHUNK_SIZE_X; y++){
        for(uint32_t x = 0; x < CHUNK_SIZE_Y; x++){
            int idx = x + y * CHUNK_SIZE_X;
            auto t = layer2[idx];
            if (t != 0) {
                auto b = biome[idx];

                if (b < 4) {
                    Rendering::RenderContext::get().matrix_clear();
                    Rendering::RenderContext::get().matrix_translate(
                        glm::vec3(
                            cX * TILE_SIZE * CHUNK_SIZE_X + ((float)x + 0.5f) * TILE_SIZE,
                            0,
                            cY * TILE_SIZE * CHUNK_SIZE_Y + ((float)y + 0.5f) * TILE_SIZE)
                    );
                    Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
                    Rendering::RenderContext::get().matrix_translate({ (-0.5f) * TILE_SIZE, 0, 0 });
                    flora[b + (t - 1) * 4]->draw();
                }
            }
        }
    }
    

}