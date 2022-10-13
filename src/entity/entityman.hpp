#pragma once
#include "entity.hpp"
#include "../player/player.hpp"
class Player;
class Enemy;
class ItemDrop;

class EntityManager final {
    public:
    EntityManager(RefPtr<Player> p);
    ~EntityManager();

    auto create_random_slime(glm::vec3 pos) -> void;
    auto create_drop(glm::vec3 pos, uint8_t item, uint16_t count, uint16_t random) -> void;

    auto tick(World* wrld) -> void;
    auto update(World* wrld, double dt) -> void;
    auto draw() -> void;
	auto player_hit() -> void;

    private:
    RefPtr<Player> player;
    std::map<u32, RefPtr<Enemy>> entities;
    std::map<u32, RefPtr<ItemDrop>> drops;
    
    u32 slimeTex;
    RefPtr<Graphics::G2D::Sprite> slimeSprites[4];
    u32 itemID;
    RefPtr<Graphics::G2D::Sprite> items[64];

    u32 ecount;
    u32 icount;

    u32 mobCap;
    u32 totalCap;
};