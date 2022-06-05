//
// Created by Wojtek on 05.06.2022.
//

#ifndef DEEPER_SOUNDSYSTEM_H
#define DEEPER_SOUNDSYSTEM_H

#include "fmod.hpp"
#include "fmod_errors.h"

class SoundSystem
{
private:
    FMOD::System *system = nullptr;
public:
    SoundSystem();

    virtual ~SoundSystem();
};

#endif //DEEPER_SOUNDSYSTEM_H
