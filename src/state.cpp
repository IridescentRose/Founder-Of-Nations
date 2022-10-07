#include "state.hpp"

auto GameState::on_start() -> void {
    SC_APP_INFO("HELLO WORLD!");

    world = create_scopeptr<World>();
    world->generate();

    camera = create_scopeptr<Rendering::Camera>(
        glm::vec3(0, 80, 0), 
        glm::vec3(15.0f / 180.0f * 3.14159f, -15.0f / 180.0f * 3.14159f, 0), 
        60.0f / 180.0f * 3.14159, 
        16.0f / 9.0f, 
        0.3f, 
        128.0f * 16.0f
    );

    Rendering::RenderContext::get().set_mode_3D();

    character = create_scopeptr<Graphics::G2D::Sprite>(
        Rendering::TextureManager::get().load_texture("./assets/char.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, true, true),
        Rendering::Rectangle{ {0, 0}, {32, 32} }
    );
}

auto GameState::on_cleanup() -> void {

}

auto GameState::on_update(Core::Application *app, double dt) -> void {
    camera->update();
    world->update(dt);
}

auto GameState::on_draw(Core::Application *app, double dt) -> void {
    world->draw();

    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_rotate({ 0, 0, 0 });
    Rendering::RenderContext::get().matrix_translate({ 0 * 32, 0, -5 * 32 });
    character->draw();
}
