#include "Audio/Sound.hpp"

std::unique_ptr<ma_engine> pl::Sound::soundEngine;

pl::Sound::Sound()
{
    if (!soundEngine)
    {
        soundEngine = std::make_unique<ma_engine>();
        ma_result result = ma_engine_init(NULL, soundEngine.get());
        if (result != MA_SUCCESS)
        {
            printf("ERROR: Failed to initialise miniaudio\n");
            soundEngine.reset();
        }
    }
}

pl::Sound::~Sound()
{
    if (sound)
    {
        ma_sound_uninit(sound.get());
    }
}

void pl::Sound::deinit()
{
    if (soundEngine)
    {
        ma_engine_uninit(soundEngine.get());
        soundEngine.reset();
    }
}

bool pl::Sound::loadFromFile(const std::string& soundPath)
{
    if (sound)
    {
        ma_sound_uninit(sound.get());
        sound.reset();
    }

    sound = std::make_unique<ma_sound>();

    ma_result result = ma_sound_init_from_file(soundEngine.get(), soundPath.c_str(), 0, NULL, NULL, sound.get());
    if (result != MA_SUCCESS)
    {
        printf("ERROR: Failed to load sound \"%s\"\n", soundPath.c_str());
        sound.reset();
        return false;
    }

    return true;
}

void pl::Sound::play()
{
    if (sound)
    {
        ma_sound_seek_to_pcm_frame(sound.get(), 0);
        ma_sound_start(sound.get());
    }
}

void pl::Sound::stop()
{
    if (sound)
    {
        ma_sound_stop(sound.get());
    }
}