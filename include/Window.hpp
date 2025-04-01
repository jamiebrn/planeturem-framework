#pragma once

#include <gl.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <string>

#include "RenderTarget.hpp"

namespace pl
{

class Window : public RenderTarget
{
public:
    Window(const std::string& title, int width, int height, uint32_t flags);
    ~Window();

    void recreate(const std::string& title, int width, int height, uint32_t flags);

    int pollEvent(SDL_Event& event);

    void swapBuffers();

    void setTitle(const std::string& title);

    virtual void bind() override;

    virtual int getWidth() override;
    virtual int getHeight() override;

private:
    SDL_Window* window;
    SDL_GLContext glContext;

    static bool glInitialised;

};

}