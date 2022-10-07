#include "state.hpp"

auto GameState::on_start() -> void {
    SC_APP_INFO("HELLO WORLD!");
}

auto GameState::on_cleanup() -> void {

}

auto GameState::on_update(Core::Application *app, double dt) -> void {

}

auto GameState::on_draw(Core::Application *app, double dt) -> void {

}
