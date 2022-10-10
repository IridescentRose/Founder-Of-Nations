#include "player.hpp"

auto Player::move_up(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->acc.x += -sinf(degtorad(p->rot)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_down(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->acc.x += sinf(degtorad(p->rot)) * PLAYER_ACCELERATION;
        p->acc.z += cosf(degtorad(p->rot)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_left(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->facing = false;
        p->acc.x += -sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_right(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->facing = true;
        p->acc.x += sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_tiltL(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        p->rot += 1.0f;
    }
}
auto Player::move_tiltR(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
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


auto Player::hit(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
        if(!p->swing){
            p->swingTimer = 1.25f / 2.0f;
            p->swing = true;
        }
    }
}