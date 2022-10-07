#include <Stardust-Celeste.hpp>
#include "state.hpp"

using namespace Stardust_Celeste;

class GameApplication : public Core::Application {
  public:
    void on_start() override {
        state = create_refptr<GameState>();
        this->set_state(state);

        Rendering::Color clear;
        clear.color = 0xFFFFCC88;

        Rendering::RenderContext::get().set_color(clear);
        Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -30, 30);
        Rendering::RenderContext::get().set_mode_2D();
    }

  private:
    RefPtr<GameState> state;
};

Core::Application *CreateNewSCApp() {
    Core::AppConfig config;
    config.headless = false;
    config.render_settings.title = "PSPVille";
    config.render_settings.width = 960;
    config.render_settings.height = 544;

    Core::PlatformLayer::get().initialize(config);

    return new GameApplication();
}
