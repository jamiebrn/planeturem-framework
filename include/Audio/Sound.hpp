#pragma once

#include <string>
#include <stdio.h>
#include <memory>
#include <extlib/miniaudio.h>

namespace pl
{

class Sound
{
public:
    Sound();
    ~Sound();
    Sound(const Sound& sound) = delete;

    static void deinit();

    bool loadFromFile(const std::string& soundPath);

    void play();
    void stop();

private:
    static std::unique_ptr<ma_engine> soundEngine;

    std::unique_ptr<ma_sound> sound;

};

}