#pragma once
#include <glm.hpp>
#include <cstdint>

class World;

class Entity {
public:
	Entity() : pos(0), vel(0), acc(0), base_atk(5), atk(5), base_def(5), def(5), base_energy(0), energy(0), base_hp(0), hp(0), next_xp(100), xp(0), iframes(0), immortal(false), in_water(false) {}
	virtual ~Entity() = default;

	glm::vec3 pos, vel, acc;

	auto update(World* wrld, double dt) -> void;
	
	auto tick() -> void;
	auto take_damage(Entity* e) -> void;

	uint16_t base_atk, atk;
	uint16_t base_def, def;

	uint16_t base_energy, energy;
	uint16_t base_hp, hp;
	uint16_t next_xp, xp;

	uint16_t iframes;
	bool immortal;
	bool in_water;

protected:

};