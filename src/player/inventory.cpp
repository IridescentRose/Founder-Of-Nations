#include "inventory.hpp"
#include <Utilities/Logger.hpp>

Inventory::Inventory() {
    for(int i = 0; i < 24; i++){
        slotData[i].count = 0;
        slotData[i].itemID = Item::None;
    }
    slotSel = -1;
}

auto Inventory::get_slot(uint8_t idx) -> Slot& {
    return slotData[idx];
}

auto Inventory::set_slot(uint8_t idx, Slot slot) -> void {
    slotData[idx] = slot;
}

auto Inventory::find_slot_existing(uint8_t itemData) -> uint8_t {
    for (int i = 0; i < 24; i++) {
        auto t = slotData[i].itemID;
        if (t == itemData)
            return i;
    }

    return 255;
}


auto Inventory::add_item(Slot slot) -> void {
    auto idx = find_slot_existing(slot.itemID);

    if (idx == 255) {
        for (int i = 0; i < 24; i++) {
            if (slotData[i].itemID == Item::None) {
                idx = i;
                slotData[idx].itemID = slot.itemID;
                break;
            }
        }
    }

    if (idx < 24) {
        slotData[idx].count += slot.count;
    }
}