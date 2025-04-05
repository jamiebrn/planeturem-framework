#pragma once

#include <gl.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <functional>
#include <string>
#include <chrono>

#include "RenderTarget.hpp"
#include "Image.hpp"

namespace pl
{

class Window : public RenderTarget
{
public:
    Window();
    Window(const std::string& title, int width, int height, uint32_t flags);
    ~Window();

    void create(const std::string& title, int width, int height, uint32_t flags);

    void setIcon(const Image& image);

    void setUpdateFunction(void (*updateFunction)(void*), void* updateFunctionData);
    void startUpdate();

    bool isOpen();
    void close();

    void toggleFullscreen();
    void setVSync(bool enabled);

    int pollEvent(SDL_Event& event);

    void swapBuffers();

    void setTitle(const std::string& title);

    virtual void bind() override;

    virtual int getWidth() override;
    virtual int getHeight() override;

private:
    static int eventWatch(void* data, SDL_Event* event);

    SDL_Window* window;
    SDL_GLContext glContext;

    // SDL_Surface* iconSurface;

    void (*updateFunction)(void*);
    void* updateFunctionData;

    bool running;
    int nonFullscreenWidth;
    int nonFullscreenHeight;

    bool initialised;

    static bool glInitialised;

};

}