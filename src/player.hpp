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

	glm::vec3 pos, vel;

private:
	ScopePtr<Rendering::Camera> camera;
	ScopePtr<Graphics::G2D::Sprite> character;
	u32 texture;
};
