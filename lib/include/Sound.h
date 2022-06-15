//
// Created by Wojtek on 05.06.2022.
//

#ifndef DEEPER_SOUND_H
#define DEEPER_SOUND_H

#include "fmod.hpp"
#include "fmod_errors.h"

#define SOUND_VOLUME_SILENT 0.0f; // Negative level inverts the signal
#define SOUND_VOLUME_HALF 0.5f;
#define SOUND_VOLUME_FULL 1.0f; // Larger than this will amplify

class Sound
{
private:
    static FMOD::System *system;
    FMOD::Sound *sound = nullptr;
    FMOD::Channel *channel = nullptr;
    float volume = SOUND_VOLUME_FULL;

    void setupChannel();

public:
    static void setSystem(FMOD::System *system);
    Sound(const char* path);

    virtual ~Sound();

    void play();
    void pause();
    void stop();
    void setVolume(float level);
    float getVolume();
};

#endif //DEEPER_SOUND_H
