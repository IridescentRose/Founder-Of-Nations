#pragma once
#include "tiles.hpp"

struct WorldGen {
    static auto generate(int cX, int cY, uint8_t *data, uint8_t *biome,
                         uint8_t *layer2) -> void;
};