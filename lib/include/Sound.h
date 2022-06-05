//
// Created by Wojtek on 05.06.2022.
//

#ifndef DEEPER_SOUND_H
#define DEEPER_SOUND_H

#include "fmod.hpp"
#include "fmod_errors.h"

class Sound
{
private:
    static FMOD::System *system;
    FMOD::Sound *sound = nullptr;
public:
    static void setSystem(FMOD::System *system);
    Sound(const char* path);
    void play();
};

#endif //DEEPER_SOUND_H
