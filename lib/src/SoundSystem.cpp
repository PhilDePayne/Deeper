//
// Created by Wojtek on 05.06.2022.
//

#include <cstdio>
#include "SoundSystem.h"
#include "Sound.h"
#include "LogMacros.h"

#ifdef DEEPER_SOUNDSYSTEM_CLASS_LOGS

#define CONSTRUCTOR_LOGS

#endif

SoundSystem::SoundSystem()
{
    FMOD_RESULT result;

    result = FMOD::System_Create(&system);      // Create the main system object.
    if (result != FMOD_OK)
    {

#ifdef CONSTRUCTOR_LOGS
        printf("SoundSystemClass::FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
#endif
        //TODO: Dodać wyjątek który wypierdala program
    }

    result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {

#ifdef CONSTRUCTOR_LOGS
        printf("SoundSystemClass::FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
#endif
        //TODO: Dodać wyjątek który wypierdala program
    }

    Sound::setSystem(system);
}

SoundSystem::~SoundSystem()
{
    system->release();
}
