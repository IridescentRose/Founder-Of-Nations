#pragma once
#include <Stardust-Celeste.hpp>
#include <Rendering/Camera.hpp>
#include <Graphics/2D/Sprite.hpp>

using namespace Stardust_Celeste;

const float PLAYER_SIZE = 32.0f;

class Player final {
public:
	Player();
	~Player();

	auto update(double dt) -> void;
	auto draw() -> void;

	static auto move_up(std::any a) -> void;
	static auto move_down(std::any a) -> void;
	static auto move_left(std::any a) -> void;
	static auto move_right(std::any a) -> void;
	static auto move_tiltL(std::any a) -> void;
	static auto move_tiltR(std::any a) -> void;

	glm::vec3 pos, vel, acc;
	float rot;

private:
	ScopePtr<Rendering::Camera> camera;
	ScopePtr<Graphics::G2D::Sprite> character;
	u32 texture;
};
