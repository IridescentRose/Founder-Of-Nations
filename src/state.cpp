#include "state.hpp"

auto GameState::on_start() -> void {
    SC_APP_INFO("HELLO WORLD!");

    world = create_scopeptr<World>();
    world->generate();
}

auto GameState::on_cleanup() -> void {

}

auto GameState::on_update(Core::Application *app, double dt) -> void {
    world->update(dt);
}

auto GameState::on_draw(Core::Application *app, double dt) -> void {
    world->draw();
}
