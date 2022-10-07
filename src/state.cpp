#include "state.hpp"

auto GameState::on_start() -> void {
    SC_APP_INFO("HELLO WORLD!");

    world = create_scopeptr<World>();
    world->generate();

    player = create_scopeptr<Player>();
}

auto GameState::on_cleanup() -> void {

}

auto GameState::on_update(Core::Application *app, double dt) -> void {
    player->update(dt);
    world->update(dt);
}

auto GameState::on_draw(Core::Application *app, double dt) -> void {
    world->draw();
    player->draw();
}
