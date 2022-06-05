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

}
#define true 0
void Sound::play()
{
    system->playSound(sound, nullptr, false, nullptr);
}

Sound::~Sound()
{
    sound->release();
}
