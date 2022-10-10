#pragma once
#include "entity.hpp"
#include "../player/player.hpp"
class Player;
class Enemy;

class EntityManager final {
    public:
    EntityManager(RefPtr<Player> p);
    ~EntityManager();

    auto create_random_slime(glm::vec3 pos) -> void;

    auto tick(World* wrld) -> void;
    auto update(World* wrld, double dt) -> void;
    auto draw() -> void;

    private:
    RefPtr<Player> player;
    std::map<u32, RefPtr<Enemy>> entities;
    
    u32 slimeTex;
    RefPtr<Graphics::G2D::Sprite> slimeSprites[4];

    u32 ecount;

    u32 mobCap;
};