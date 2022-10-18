#include "sfxman.hpp"

SFXManager::SFXManager() {
    {
        std::vector<RefPtr<Audio::Clip>> v;
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/pickup.wav"));
        soundmap.emplace(SFX_TYPE_PICKUP, v);
    }
    {
        std::vector<RefPtr<Audio::Clip>> v;
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/hit1.wav"));
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/hit2.wav"));
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/hit3.wav"));
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/hit4.wav"));
        soundmap.emplace(SFX_TYPE_HIT, v);
    }
    {
        std::vector<RefPtr<Audio::Clip>> v;
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/kill1.wav"));
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/kill2.wav"));
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/kill3.wav"));
        v.push_back(create_refptr<Audio::Clip>("assets/sfx/kill4.wav"));
        soundmap.emplace(SFX_TYPE_KILL, v);
    }
}

SFXManager::~SFXManager() {
    for(auto& [id, vec] : soundmap) {
        vec.clear();
    }
    soundmap.clear();
}

auto SFXManager::play(uint8_t type) -> void {
    if(soundmap.find(type) != soundmap.end()) {
        auto v = soundmap[type];
        auto r = rand() % v.size();

        v[r]->play(rand() % 7);
    }
}