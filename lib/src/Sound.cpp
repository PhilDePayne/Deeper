//
// Created by Wojtek on 05.06.2022.
//

#include "Sound.h"
#include "LogMacros.h"

#include <stdio.h>

#ifdef DEEPER_SOUND_CLASS_LOGS

#define CONSTRUCTOR_LOG

#endif

FMOD::System *Sound::system = nullptr;

void Sound::setSystem(FMOD::System *newSystem)
{
    Sound::system = newSystem;
}

Sound::Sound(const char *path)
{
    FMOD_RESULT result;
    result = system->createSound(path, FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK)
    {
#ifdef CONSTRUCTOR_LOG
        printf("SoundClass::FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
#endif
        //TODO: Jakieś info albo wyjatek, że chuj z tego
    }
    setupChannel();
}

Sound::~Sound()
{
    sound->release();
}

void Sound::play()
{
    bool isPlaying;
    channel->isPlaying(&isPlaying);
    if (isPlaying)
    {
        stop();
    }
    else
    {
        setupChannel();
    }
    channel->setPaused(false);
}

void Sound::pause()
{
    channel->setPaused(true);
}

void Sound::stop()
{
    channel->stop();
    setupChannel();
}

void Sound::setVolume(float level)
{
    volume = level;
    channel->setVolume(level);
}

float Sound::getVolume()
{
    return volume;
}

void Sound::setupChannel()
{
    system->playSound(sound, nullptr, true, &channel);
    channel->setVolume(volume);
}
