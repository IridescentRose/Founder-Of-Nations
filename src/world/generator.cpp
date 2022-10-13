#include "generator.hpp"
#include "noiseutil.hpp"

NoiseUtil::NoiseSettings settings{3, 1.25f, 0.5f, 0.9f, 4.0f, 0, 0.0f, 1.0f};

NoiseUtil::NoiseSettings settings2{2, 1.0f, 8.0f, 0.9f, 2.0f, 0, 0.0f, 1.0f};

NoiseUtil::NoiseSettings settings3{3, 1.0f, 0.8f, 0.9f, 1.25f, 0, 0.0f, 1.0f};

NoiseUtil::NoiseSettings settings4{2, 1.0f, 16.0f, 0.9f, 1.5f, 0, 0.0f, 1.0f};
NoiseUtil::NoiseSettings settings5{3, 1.0f, 8.0f, 0.9f, 2.0f, 0, 0.0f, 1.0f};

const int CHUNK_SIZE_X = 16;
const int CHUNK_SIZE_Y = 16;

auto get_biome(int cX, int cY, int x, int y) -> uint8_t {

    auto temperature =
        NoiseUtil::get_noise((int)(x + cX * CHUNK_SIZE_X) / 2,
                             (int)(y + cY * CHUNK_SIZE_Y) / 2, &settings3);
    auto humidity =
        NoiseUtil::get_noise(-(int)(x + cX * CHUNK_SIZE_X) / 2,
                             -(int)(y + cY * CHUNK_SIZE_Y) / 2, &settings3);

    if (temperature < 0.4f) {
        return Biome::Taiga;
    } else if (temperature > 0.6f) {
        return Biome::Desert;
    } else {
        if (humidity > 0.5f) {
            return Biome::Forest;
        } else {
            return Biome::Plains;
        }
    }
}

auto step_direction(int &x, int &y, float rot) {
    if (rot >= 0.0f && rot <= 45.0f) {
        x += 1;
    } else if (rot >= 45.0f && rot <= 90.0f) {
        x += 1;
        y += 1;
    } else if (rot >= 90.0f && rot <= 135.0f) {
        y += 1;
    } else if (rot >= 135.0f && rot <= 180.0f) {
        x -= 1;
        y += 1;
    } else if (rot >= 180.0f && rot <= 225.0f) {
        x -= 1;
    } else if (rot >= 225.0f && rot <= 270.0f) {
        x -= 1;
        y -= 1;
    } else if (rot >= 270.0f && rot <= 335.0f) {
        y -= 1;
    } else if (rot >= 335.0f && rot <= 360.0f) {
        x += 1;
        y -= 1;
    }
}


auto WorldGen::generate(int cX, int cY, uint8_t *data, uint8_t *biome, uint8_t *layer2) -> void {
    float base_map[256];

    // Base map & biome generation
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            auto idx = x + y * CHUNK_SIZE_X;
            base_map[idx] = NoiseUtil::get_noise(
                x + cX * CHUNK_SIZE_X, y + cY * CHUNK_SIZE_Y, &settings);

            if (base_map[idx] < 0.45f) {
                biome[idx] = Biome::Ocean;
                data[idx] = Tile::Water_Heavy;
            } else if (base_map[idx] >= 0.45f && base_map[idx] < 0.46f) {
                biome[idx] = Biome::Light_Ocean;
                data[idx] = Tile::Water_Light;
            } else if (base_map[idx] >= 0.46f && base_map[idx] < 0.475f) {
                biome[idx] = Biome::Beach;
                data[idx] = Tile::Sand;
            }
        }
    }

    // Generate land pass
    srand(cX << 16 | (cY & 0x00FF));
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            auto idx = x + y * CHUNK_SIZE_X;
            if (biome[idx] != Biome::Ocean && biome[idx] != Biome::Beach &&
                biome[idx] != Biome::Light_Ocean &&
                biome[idx] != Biome::River) {

                base_map[idx] = NoiseUtil::get_noise(
                    x + cX * CHUNK_SIZE_X, y + cY * CHUNK_SIZE_Y, &settings2);

                if (base_map[idx] > 0.70f) {
                    int check = rand() % 100;

                    if (check <= 10)
                        data[idx] = Tile::Stone_Coal;
                    //else if (check == 11)
                    //    data[idx] = Tile::Cave_Entrance;
                    else
                        data[idx] = Tile::Stone;
                } else {
                    data[idx] = Tile::Grass;
                }

                biome[idx] = get_biome(cX, cY, x, y);
            }
        }
    }



    // Try flora generation
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            auto idx = x + y * CHUNK_SIZE_X;

            if (biome[idx] == Biome::Plains && data[idx] == Tile::Grass) {
                int check = rand() % 200;

                if (check <= 50){
                    data[idx] = Tile::Grass;
                    layer2[idx] = Decorations::Tallgrass; //TALL
                }
                else if (check == 51){
                    data[idx] = Tile::Grass;
                    layer2[idx] = Decorations::Flower; //FLOWER
                }
                else if (check == 52){
                    data[idx] = Tile::Grass;
                    layer2[idx] = Decorations::Bush; //BUSH
                }
                else if (check == 53)
                    layer2[idx] = Decorations::Tree;
            }
            if ((biome[idx] == Biome::Forest || biome[idx] == Biome::Taiga) &&
                data[idx] == Tile::Grass) {
                int check = rand() % 150;

                if (check <= 1){
                    data[idx] = Tile::Grass;
                    layer2[idx] = Decorations::Flower; //FLOWER
                }else if (check > 1 && check <= 3){
                    data[idx] = Tile::Grass;
                    layer2[idx] = Decorations::Tallgrass; //TALL
                } else if (biome[idx] == Biome::Forest) {
                    if (check > 5 && check <= 7){
                        data[idx] = Tile::Grass;
                        layer2[idx] = Decorations::Bush; //BUSH
                    }
                    else if (check == 8) {
                        data[idx] = Tile::Grass;
                        layer2[idx] = Decorations::Bush; //BUSH
                    }
                } else if (check == 4) {
                        data[idx] = Tile::Grass;
                        layer2[idx] = Decorations::Bush; //BUSH
                } else if (check == 6) {
                    layer2[idx] = Decorations::Tree;
                }

                if (check > 8 && check <= 25) {
                    layer2[idx] = Decorations::Tree;
                }
            }
            if (biome[idx] == Biome::Desert && data[idx] == Tile::Grass) {
                int check = rand() % 30;

                if (check <= 1){
                    data[idx] = Tile::Grass;
                    layer2[idx] = Decorations::Tallgrass; //TALL
                }
                else if (check == 2)
                    layer2[idx] = Decorations::Tree;
            }
        }
    }
}
