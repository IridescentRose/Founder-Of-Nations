#pragma once
#include <Stardust-Celeste.hpp>
#include <Rendering/Camera.hpp>
#include <Graphics/2D/AnimatedSprite.hpp>
#include "../entity/entity.hpp"
#include "../world/world.hpp"
#include "ui.hpp"
#include "inventory.hpp"

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

	auto tick() -> void override;

	static auto move_up(std::any a) -> void;
	static auto move_down(std::any a) -> void;
	static auto move_left(std::any a) -> void;
	static auto move_right(std::any a) -> void;
	static auto move_tiltL(std::any a) -> void;
	static auto move_tiltR(std::any a) -> void;

	static auto invScrollL(std::any a) -> void;
	static auto invScrollR(std::any a) -> void;

	static auto hit(std::any a) -> void;

	float rot;
	RefPtr<Inventory> inventory;
private:
	bool triggerHit;
	bool facing;
	bool swing;
	float swingTimer;
	ScopePtr<Rendering::Camera> camera;
	ScopePtr<Graphics::G2D::AnimatedSprite> character;
	ScopePtr<UI> ui;
	u32 texture;
	s32 invSelect;
};
