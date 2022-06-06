//
// Created by Wojtek on 05.06.2022.
//

#include "Sound.h"
#include "LogMacros.h"

#ifdef DEEPER_SOUND_CLASS_LOGS



#endif

FMOD::System *Sound::system = nullptr;

void Sound::setSystem(FMOD::System *newSystem)
{
    Sound::system = newSystem;
}

Sound::Sound(const char *path)
{
    FMOD_RESULT result;
    result = system->createSound("./res/sounds/sound.mp3", FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK)
    {
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
    channel->setVolume(level);
}

float Sound::getVolume()
{
    float volume;
    channel->getVolume(&volume);
    return volume;
}

void Sound::setupChannel()
{
    system->playSound(sound, nullptr, true, &channel);
}
