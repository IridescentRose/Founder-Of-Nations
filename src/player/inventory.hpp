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
    const uint8_t Berries = 8;
    const uint8_t Stone = 9;
    const uint8_t Coal = 10;
    const uint8_t Log = 11;
    const uint8_t Acorn = 12;
    const uint8_t Seed = 13;
    const uint8_t Sand = 14;

    const uint8_t GoldenApple = 16;
    const uint8_t Bow = 21;
}

struct Slot {
    uint8_t itemID;
    uint16_t count;
};

class Inventory final {
    public:
    Inventory();
    ~Inventory() = default;

    auto get_slot(uint8_t idx) -> Slot&;
    auto set_slot(uint8_t idx, Slot slot) -> void;

    auto add_item(Slot slot) -> void;

    private:
    auto find_slot_existing(uint8_t itemData) -> uint8_t;
    std::array<Slot, 24> slotData;
};