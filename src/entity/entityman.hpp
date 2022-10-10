#pragma once
#include "entity.hpp"
#include "../player/player.hpp"
class Player;

class EntityManager final {
    public:
    EntityManager(RefPtr<Player> p);
    ~EntityManager();

    auto create_random_slime(glm::vec3 pos) -> void;

    auto update(World* wrld, double dt) -> void;
    auto draw() -> void;

    private:
    RefPtr<Player> player;
    std::map<u32, RefPtr<Entity>> entities;
    
    u32 slimeTex;
    RefPtr<Graphics::G2D::Sprite> slimeSprites[4];

    u32 ecount;
};