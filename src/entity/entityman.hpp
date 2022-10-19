#pragma once
#include "entity.hpp"
#include "../player/player.hpp"
class Player;
class Enemy;
class ItemDrop;
class ArrowEntity;
class Human;

class EntityManager final {
    public:
    EntityManager(RefPtr<Player> p);
    ~EntityManager();

    auto create_random_slime(glm::vec3 pos) -> void;
    auto create_random_spider(glm::vec3 pos) -> void;
    auto create_random_beholder(glm::vec3 pos) -> void;
    auto create_drop(glm::vec3 pos, uint8_t item, uint16_t count, uint16_t random) -> void;
    auto create_human(glm::vec3 pos, uint8_t type) -> void;

    auto tick(World* wrld) -> void;
    auto update(World* wrld, double dt) -> void;
    auto draw() -> void;
	auto player_hit() -> void;
    auto player_arrow(bool facing, glm::vec3 pos, glm::vec3 vel, uint32_t arrowATK) -> void;

    private:
    RefPtr<Player> player;
    std::map<u32, RefPtr<ArrowEntity>> arrows;
    std::map<u32, RefPtr<Enemy>> entities;
    std::map<u32, RefPtr<ItemDrop>> drops;
    std::map<u32, RefPtr<Human>> humans;

    u32 humanTex;

    u32 arrowTex;
    RefPtr<Graphics::G2D::Sprite> arrowSprite;
    u32 slimeTex;
    RefPtr<Graphics::G2D::Sprite> slimeSprites[4];
    u32 spiderTex;
    RefPtr<Graphics::G2D::Sprite> spiderSprites[4];
    u32 beholderTex;
    RefPtr<Graphics::G2D::Sprite> beholderSprites[4];
    u32 itemID;
    RefPtr<Graphics::G2D::Sprite> items[64];

    u32 acount;
    u32 ecount;
    u32 icount;
    u32 hcount;

    s32 mobCap;
    u32 totalCap;
};