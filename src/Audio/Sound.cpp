#include "Audio/Sound.hpp"

std::unique_ptr<ma_resource_manager> pl::Sound::resourceManager;
std::unique_ptr<ma_engine> pl::Sound::soundEngine;

pl::Sound::Sound()
{
    if (!soundEngine)
    {
        initEngine();
    }

    sound = nullptr;
}

pl::Sound::~Sound()
{
    if (sound)
    {
        ma_sound_uninit(sound.get());
    }
}

void pl::Sound::initEngine()
{
    if (!resourceManager)
    {
        resourceManager = std::make_unique<ma_resource_manager>();
        ma_resource_manager_config config = ma_resource_manager_config_init();
        config.ppCustomDecodingBackendVTables = &ma_decoding_backend_libvorbis;
        config.customDecodingBackendCount = 1;
        config.pCustomDecodingBackendUserData = NULL;

        ma_result result = ma_resource_manager_init(&config, resourceManager.get());
        if (result != MA_SUCCESS)
        {
            printf("ERROR: Failed to initialise miniaudio resource manager\n");
            resourceManager.reset();
            return;
        }
    }

    soundEngine = std::make_unique<ma_engine>();

    ma_engine_config config = ma_engine_config_init();
    config.pResourceManager = resourceManager.get();

    ma_result result = ma_engine_init(&config, soundEngine.get());

    if (result != MA_SUCCESS)
    {
        printf("ERROR: Failed to initialise miniaudio engine\n");
        soundEngine.reset();
    }
}

void pl::Sound::deinit()
{
    if (soundEngine)
    {
        ma_engine_uninit(soundEngine.get());
        soundEngine.reset();
    }

    if (resourceManager)
    {
        ma_resource_manager_uninit(resourceManager.get());
        resourceManager.reset();
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

bool pl::Sound::isPlaying()
{
    if (sound)
    {
        return ma_sound_is_playing(sound.get());
    }
    return false;
}

bool pl::Sound::isFinished()
{
    if (sound)
    {
        return ma_sound_at_end(sound.get());
    }
    return false;
}

void pl::Sound::setVolume(float volume)
{
    if (sound)
    {
        ma_sound_set_volume(sound.get(), volume);
    }
}

float pl::Sound::getVolume()
{
    if (sound)
    {
        return ma_sound_get_volume(sound.get());
    }
    return 0.0f;
}