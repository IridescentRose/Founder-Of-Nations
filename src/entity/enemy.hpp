#include "entity.hpp"

#include <Graphics/2D/Sprite.hpp>
using namespace Stardust_Celeste;

class Enemy : public Entity {
public:
	Enemy(RefPtr<Graphics::G2D::Sprite> model) : acceleration_speed(30), sprt(model) {};
    virtual ~Enemy() = default;

    auto update_enemy(World* wrld, double dt, glm::vec3 player_pos) -> void;
    auto draw(float rot) -> void;
protected:
    float acceleration_speed;
    RefPtr<Graphics::G2D::Sprite> sprt;
};