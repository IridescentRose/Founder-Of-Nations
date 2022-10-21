#pragma once
#include <Audio/Clip.hpp>
#include <Utilities/Singleton.hpp>
#include <vector>
#include <map>

using namespace Stardust_Celeste;

enum MUSType{
    MUS_TYPE_M1 = 0,
    MUS_TYPE_M2 = 1,
    MUS_TYPE_M3 = 2,
    MUS_TYPE_M4 = 3,
    MUS_TYPE_DEATH = 4,
    MUS_TYPE_INTRO = 5,
    MUS_TYPE_BOSS = 6
};

class MUSManager final : public Singleton {
    public:
    MUSManager();
    ~MUSManager();

    auto play(uint8_t type) -> void;
    auto update() -> void;

    static inline auto get() -> MUSManager& {
        static MUSManager sfxm;
        return sfxm;
    }

    private:
    int active;
    std::map<MUSType, RefPtr<Audio::Clip>> soundmap;
};