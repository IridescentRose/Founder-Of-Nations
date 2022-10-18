#pragma once
#include <Audio/Clip.hpp>
#include <Utilities/Singleton.hpp>
#include <vector>
#include <map>

using namespace Stardust_Celeste;

enum SFXType{
    SFX_TYPE_HIT,
    SFX_TYPE_KILL,
    SFX_TYPE_PICKUP
};

class SFXManager final : public Singleton {
    public:
    SFXManager();
    ~SFXManager();

    auto play(uint8_t type) -> void;

    static inline auto get() -> SFXManager& {
        static SFXManager sfxm;
        return sfxm;
    }

    private:
    std::map<uint8_t, std::vector<RefPtr<Audio::Clip>>> soundmap;
};