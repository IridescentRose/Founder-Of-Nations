#include "arrow.hpp"

auto ArrowEntity::update_arrow(World* wrld, double dt) -> void {
    timer += dt;
    if (timer < 0.6f)
        return;
    vel = fixedVel;
    Entity::update(wrld, dt);
}

auto ArrowEntity::draw(float rot) -> void {
    if (timer < 0.75f)
        return;
        
#ifndef PSP
    glDisable(GL_CULL_FACE);
#else 
    sceGuDisable(GU_CULL_FACE);
#endif

    Rendering::RenderContext::get().matrix_clear();
    Rendering::RenderContext::get().matrix_translate({ (pos.x) * 32.0f, pos.y * 32.0f, pos.z * 32.0f });
    if(!facing)
        Rendering::RenderContext::get().matrix_rotate({ 0, rot + 180, 0 });
    else
        Rendering::RenderContext::get().matrix_rotate({ 0, rot, 0 });
    Rendering::RenderContext::get().matrix_translate({ (-0.5f) * 32.0f, 0, 0 });

    sprite->draw();

#ifndef PSP
    glEnable(GL_CULL_FACE);
#else 
    sceGuEnable(GU_CULL_FACE);
#endif

}