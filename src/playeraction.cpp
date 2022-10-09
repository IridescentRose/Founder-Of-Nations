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
        p->acc.x += -sinf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
        p->acc.z += -cosf(degtorad(p->rot + 90.0f)) * PLAYER_ACCELERATION;
    }
}
auto Player::move_right(std::any a) -> void {
    Player* p = std::any_cast<Player*>(a);

    if (p != nullptr) {
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