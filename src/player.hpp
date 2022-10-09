#pragma once
#include <Stardust-Celeste.hpp>
#include <Rendering/Camera.hpp>
#include <Graphics/2D/Sprite.hpp>
#include "entity.hpp"
#include "world.hpp"

using namespace Stardust_Celeste;

const float PLAYER_SIZE = 32.0f;
const float PLAYER_ACCELERATION = 42.069f;

inline auto degtorad(float x) -> float {
	return x / 180.0f * 3.14159f;
}

class World;
class Entity;

class Player final : public Entity {
public:
	Player();
	~Player();

	auto update(World* wrld, double dt) -> void;
	auto draw() -> void;

	static auto move_up(std::any a) -> void;
	static auto move_down(std::any a) -> void;
	static auto move_left(std::any a) -> void;
	static auto move_right(std::any a) -> void;
	static auto move_tiltL(std::any a) -> void;
	static auto move_tiltR(std::any a) -> void;

	float rot;

private:
	ScopePtr<Rendering::Camera> camera;
	ScopePtr<Graphics::G2D::Sprite> character;
	u32 texture;
};
