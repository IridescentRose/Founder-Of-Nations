#pragma once
#include <array>
#include <cstdint>

namespace Item {
    const uint8_t None = 0;
    const uint8_t Sword = 1;
    const uint8_t Pickaxe = 2;
    const uint8_t Hoe = 3;
    const uint8_t Axe = 4;
    const uint8_t Shovel = 5;
    const uint8_t Wheat = 6;
    const uint8_t Bread = 7;
    const uint8_t Apple = 8;
}

struct Slot {
    uint8_t itemID;
    uint8_t count;
};

class Inventory final {
    public:
    Inventory();
    ~Inventory() = default;

    auto get_slot(uint8_t idx) -> Slot&;
    auto set_slot(uint8_t idx, Slot slot) -> void;

    private:
    std::array<Slot, 24> slotData;
};