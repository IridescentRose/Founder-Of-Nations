#include "player.hpp"
#include <Utilities/Input.hpp>

auto Player::move_up(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr && !p->inInventory) {
        p->acc.x += -sinf(degtorad(p->rot)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_down(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr && !p->inInventory) {
        p->acc.x += sinf(degtorad(p->rot)) * PLAYER_ACCELERATION;
        p->acc.z += cosf(degtorad(p->rot)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_left(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr && !p->inInventory) {
        p->facing = false;
        p->acc.x += -sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_right(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr && !p->inInventory) {
        p->facing = true;
        p->acc.x += sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_tiltL(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr && !p->inInventory) {
        p->rot += 1.0f;
    }
}
auto Player::move_tiltR(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr && !p->inInventory) {
        p->rot -= 1.0f;
    }
}

auto Player::invScrollL(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->invSelect--;

        if(p->invSelect < 0)
            p->invSelect = 5;
    }
}
auto Player::invScrollR(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->invSelect++;

        if(p->invSelect > 5)
            p->invSelect = 0;
    }
}


auto Player::toggle_inv(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);
    if (p != nullptr)
        p->inInventory = !p->inInventory;
}

auto Player::hit(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        if (!p->inInventory) {
            if (!p->swing && p->energy > 15) {
                if (p->inventory->get_slot(p->invSelect).itemID == Item::Bow) {
                    p->swingTimer = 2.25f / 2.0f;
                }
                else {
                    p->swingTimer = 1.25f / 2.0f;
                }

                p->swing = true;
                p->triggerHit = true;
                if (p->inventory->get_slot(p->invSelect).itemID == Item::Sword) {
                    p->energy -= 7;
                }
                else {
                    p->energy -= 15;
                }
            }
        }
        else {
#ifndef PSP
            int cX = Utilities::Input::get_axis("Mouse", "X") * 480;
            int cY = (1.0f - Utilities::Input::get_axis("Mouse", "Y")) * 272;

            cX -= 168;
            cX /= 24;

            cY /= 32;

            if (cX >= 0 && cX < 6 && cY >= 0 && cY < 4) {
                auto idx = cX + cY * 6;

                if (p->inventory->slotSel != -1) {
                    //Swap
                    Slot a = p->inventory->get_slot(p->inventory->slotSel);
                    Slot b = p->inventory->get_slot(idx);
                    p->inventory->set_slot(idx, a);
                    p->inventory->set_slot(p->inventory->slotSel, b);
                    p->inventory->slotSel = -1;
                }
                else {
                    p->inventory->slotSel = idx;
                }
            }
#endif
        }
    }
}