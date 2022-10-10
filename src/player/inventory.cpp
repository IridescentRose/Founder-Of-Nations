#include "inventory.hpp"

Inventory::Inventory() {
    for(int i = 0; i < 24; i++){
        slotData[i].count = 0;
        slotData[i].count = Item::None;
    }
}

auto Inventory::get_slot(uint8_t idx) -> Slot& {
    return slotData[idx];
}

auto Inventory::set_slot(uint8_t idx, Slot slot) -> void {
    slotData[idx] = slot;
}