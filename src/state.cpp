#include "state.hpp"
#include <Utilities/Controllers/KeyboardController.hpp>
#include <Utilities/Controllers/MouseController.hpp>
#include <Utilities/Controllers/PSPController.hpp>
#include "musman.hpp"

#ifndef PSP
#include <GLFW/glfw3.h>
#endif

auto GameState::on_start() -> void {
    srand(time(NULL));
    player = create_refptr<Player>();
    world = create_scopeptr<World>(player);

    using namespace Utilities;

    psp_controller = new Input::PSPController();
    key_controller = new Input::KeyboardController();
    mouse_controller = new Input::MouseController();

    psp_controller->add_command({ (int)Input::PSPButtons::LTrigger, KeyFlag::Press | KeyFlag::Held }, { Player::move_tiltL, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::RTrigger, KeyFlag::Press | KeyFlag::Held }, { Player::move_tiltR, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Left, KeyFlag::Press }, { Player::invScrollL, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Right, KeyFlag::Press }, { Player::invScrollR, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Up, KeyFlag::Press }, { Player::invScrollUp, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Down, KeyFlag::Press }, { Player::invScrollDown, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Triangle, KeyFlag::Press }, { Player::toggle_inv, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Cross, KeyFlag::Press }, { Player::hit, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Square, KeyFlag::Press }, { Player::use, player.get() });
    psp_controller->add_command({ (int)Input::PSPButtons::Start, KeyFlag::Press }, { World::save_game, world.get() });

    key_controller->add_command({ (int)Input::Keys::W, KeyFlag::Press | KeyFlag::Held }, { Player::move_up, player.get() });
    key_controller->add_command({ (int)Input::Keys::S, KeyFlag::Press | KeyFlag::Held }, { Player::move_down, player.get() });
    key_controller->add_command({ (int)Input::Keys::A, KeyFlag::Press | KeyFlag::Held }, { Player::move_left, player.get() });
    key_controller->add_command({ (int)Input::Keys::D, KeyFlag::Press | KeyFlag::Held }, { Player::move_right, player.get() });
    key_controller->add_command({ (int)Input::Keys::Q, KeyFlag::Press | KeyFlag::Held }, { Player::move_tiltL, player.get() });
    key_controller->add_command({ (int)Input::Keys::E, KeyFlag::Press | KeyFlag::Held }, { Player::move_tiltR, player.get() });
    key_controller->add_command({ (int)Input::Keys::I, KeyFlag::Press }, { Player::toggle_inv, player.get() });
    key_controller->add_command({ (int)Input::Keys::Z, KeyFlag::Press }, { Player::invScrollL, player.get() });
    key_controller->add_command({ (int)Input::Keys::C, KeyFlag::Press }, { Player::invScrollR, player.get() });
    key_controller->add_command({ (int)Input::Keys::Enter, KeyFlag::Press }, { World::save_game, world.get() });

    mouse_controller->add_command({ (int)Input::MouseButtons::Left, KeyFlag::Press }, { Player::hit, player.get() });
    mouse_controller->add_command({ (int)Input::MouseButtons::Right, KeyFlag::Press }, { Player::use, player.get() });

    Input::add_controller(psp_controller);
    Input::add_controller(key_controller);
    Input::add_controller(mouse_controller);

    tick_timer = 0.0f;
    
#ifndef PSP
    glfwSwapInterval(1);
#endif
}

auto GameState::on_cleanup() -> void {

}

auto GameState::on_update(Core::Application *app, double dt) -> void {
    Utilities::Input::update();
    MUSManager::get().update();

    player->update(world.get(), dt);
    world->update(dt);

    tick_timer += dt;
    if(tick_timer > 0.5f){
        tick_timer = 0.0f;
        world->tick();
    }
}

auto GameState::on_draw(Core::Application *app, double dt) -> void {
    world->draw();
    player->draw();
}
