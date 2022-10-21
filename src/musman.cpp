#include "musman.hpp"

#ifdef PSP
const std::string PLATFORM_PREFIX = ".bgm";
#else 
const std::string PLATFORM_PREFIX = ".ogg";
#endif

MUSManager::MUSManager() {
    soundmap.emplace(MUS_TYPE_M1, create_refptr<Audio::Clip>("assets/mus/m1" + PLATFORM_PREFIX, true));
    soundmap.emplace(MUS_TYPE_M2, create_refptr<Audio::Clip>("assets/mus/m2" + PLATFORM_PREFIX, true));
    soundmap.emplace(MUS_TYPE_M3, create_refptr<Audio::Clip>("assets/mus/m3" + PLATFORM_PREFIX, true));
    soundmap.emplace(MUS_TYPE_M4, create_refptr<Audio::Clip>("assets/mus/m4" + PLATFORM_PREFIX, true));
    soundmap.emplace(MUS_TYPE_DEATH, create_refptr<Audio::Clip>("assets/mus/death" + PLATFORM_PREFIX, true));
    soundmap.emplace(MUS_TYPE_INTRO, create_refptr<Audio::Clip>("assets/mus/intro" + PLATFORM_PREFIX, true));
    soundmap.emplace(MUS_TYPE_BOSS, create_refptr<Audio::Clip>("assets/mus/boss" + PLATFORM_PREFIX, true));
    active = -1;
}

MUSManager::~MUSManager() {
    soundmap.clear();
}


auto MUSManager::update() -> void {
    if (active != -1)
        soundmap[(MUSType)active]->update();
}

auto MUSManager::play(uint8_t type) -> void {
    if(soundmap.find((MUSType)type) != soundmap.end()) {
        if (active != -1)
            soundmap[(MUSType)active]->stop();

        soundmap[(MUSType)type]->play();
        active = type;
    }
}