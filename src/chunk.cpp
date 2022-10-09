#include "chunk.hpp"
#include "generator.hpp"

const uint32_t CHUNK_SIZE_X = 16;
const uint32_t CHUNK_SIZE_Y = 16;
const uint32_t TILE_SIZE = 32;


Chunk::Chunk(int cx, int cy, u32 terrain, u32 tree) : cX(cx), cY(cy) {
    tmap = create_scopeptr<AnimatedTilemap>(
        terrain,
        glm::vec2{16, 16});

    for(int i = 0; i < 16; i++){
        flora[i] = create_scopeptr<Sprite>(
            tree,
            Rendering::Rectangle{ {0, 0}, {16 + 8, 48 + 24}},
            Rendering::Rectangle{ {0.25f * (i % 4), 0.75 - (i / 4) * 0.25f}, {0.25, 0.25}}
        );
    }

    tmap->tickPerSec = 2.0f;

    for (auto i = 0; i < 256; i++) {
        tiles[i] = 0;
        biome[i] = 0;
        layer2[i] = 0;
    }
}

auto Chunk::update(float dt) -> void {
    tmap->update(dt);
}
struct TileData {
    u8 start_frame;
    u8 frame_count;
};

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
        return {21, 1};
    case Tile::Cave_Entrance:
        return {22, 1};
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

    default:
        return {id, 1};
    }
}

auto create_tile(uint8_t id, uint8_t biome, glm::vec2 pos) -> AnimatedTile {
    auto data = get_tile_data(id, biome);
    return Graphics::G2D::AnimatedTile {
        Rendering::Rectangle{glm::vec2{pos.x * TILE_SIZE, pos.y * TILE_SIZE},
                             glm::vec2{TILE_SIZE, TILE_SIZE}},
        Rendering::Color { 0xFF, 0xFF, 0xFF, 0xFF },
        data.start_frame,
        0.0f,
        data.start_frame,
        data.frame_count,
    };
}

auto Chunk::generate_tile_data() -> void {
    WorldGen::generate(cX, cY, tiles, biome, layer2);
}

auto Chunk::generate_mesh_data() -> void {
    tmap->clear_tiles();
    
    for (int x = 0; x < CHUNK_SIZE_X; x++)
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            auto idx = x + y * CHUNK_SIZE_X;
            tmap->add_tile(create_tile(tiles[idx], biome[idx], {x, y}));
            //TODO: Layer2 Flora
        }

    tmap->generate_map();
}

auto Chunk::draw(float rot) -> void {
    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate(glm::vec3(cX * TILE_SIZE * CHUNK_SIZE_X, cY * TILE_SIZE * CHUNK_SIZE_Y, 0));
    Rendering::RenderContext::get().matrix_rotate({-90, 0, -90});
    tmap->draw();

    for(uint32_t y = 0; y < CHUNK_SIZE_X; y++){
        for(uint32_t x = 0; x < CHUNK_SIZE_Y; x++){
            int idx = x + y * CHUNK_SIZE_X;

            auto t = layer2[idx];
            if(t != 0){
                auto b = biome[idx];
                if(t == 1){
                    if(b < 4){
                        Rendering::RenderContext::get().matrix_clear();
                        Rendering::RenderContext::get().matrix_translate(glm::vec3(cX * TILE_SIZE * CHUNK_SIZE_X + ((float)y + 0.5f) * TILE_SIZE, 0, cY * TILE_SIZE * CHUNK_SIZE_Y + ((float)x + 0.5f) * TILE_SIZE));
                        Rendering::RenderContext::get().matrix_rotate({0.0f, rot, 0});
                        Rendering::RenderContext::get().matrix_translate({ (-0.5f) * TILE_SIZE, 0, 0});
                        flora[b]->draw();
                    }
                }
                if(t == 2){
                    if(b < 4){
                        Rendering::RenderContext::get().matrix_clear();
                        Rendering::RenderContext::get().matrix_translate(glm::vec3(cX * TILE_SIZE * CHUNK_SIZE_X + ((float)y + 0.5f) * TILE_SIZE, 0, cY * TILE_SIZE * CHUNK_SIZE_Y + ((float)x + 0.5f) * TILE_SIZE));
                        Rendering::RenderContext::get().matrix_rotate({0.0f, rot, 0});
                        Rendering::RenderContext::get().matrix_translate({ (-0.5f) * TILE_SIZE, 0, 0});
                        flora[b + 4]->draw();
                    }
                }
                if(t == 3){
                    if(b < 4){
                        Rendering::RenderContext::get().matrix_clear();
                        Rendering::RenderContext::get().matrix_translate(glm::vec3(cX * TILE_SIZE * CHUNK_SIZE_X + ((float)y + 0.5f) * TILE_SIZE, 0, cY * TILE_SIZE * CHUNK_SIZE_Y + ((float)x + 0.5f) * TILE_SIZE));
                        Rendering::RenderContext::get().matrix_rotate({0.0f, rot, 0});
                        Rendering::RenderContext::get().matrix_translate({ (-0.5f) * TILE_SIZE, 0, 0});
                        flora[b + 8]->draw();
                    }
                }
                if(t == 5){
                    if(b < 4){
                        Rendering::RenderContext::get().matrix_clear();
                        Rendering::RenderContext::get().matrix_translate(glm::vec3(cX * TILE_SIZE * CHUNK_SIZE_X + ((float)y + 0.5f) * TILE_SIZE, 0, cY * TILE_SIZE * CHUNK_SIZE_Y + ((float)x + 0.5f) * TILE_SIZE));
                        Rendering::RenderContext::get().matrix_rotate({0.0f, rot, 0});
                        Rendering::RenderContext::get().matrix_translate({ (-0.5f) * TILE_SIZE, 0, 0});
                        flora[b + 12]->draw();
                    }
                }
            }
        }
    }
}