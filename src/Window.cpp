#include "Window.hpp"

bool pl::Window::glInitialised = false;

pl::Window::Window(const std::string& title, int width, int height, uint32_t flags)
{
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | flags);
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    if (!glInitialised)
    {
        gladLoaderLoadGL();
        glInitialised = true;
        glEnable(GL_BLEND);
    }
}

pl::Window::~Window()
{
    SDL_DestroyWindow(window);
}

void pl::Window::recreate(const std::string& title, int width, int height, uint32_t flags)
{
    SDL_DestroyWindow(window);
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | flags);
    SDL_GL_MakeCurrent(window, glContext);
}

int pl::Window::pollEvent(SDL_Event& event)
{
    int eventReceived = SDL_PollEvent(&event);

    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            glViewport(0, 0, event.window.data1, event.window.data2);
        }
    }

    return eventReceived;
}

void pl::Window::swapBuffers()
{
    SDL_GL_SwapWindow(window);
}

void pl::Window::setTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void pl::Window::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int pl::Window::getWidth()
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    return windowWidth;
}

int pl::Window::getHeight()
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    return windowHeight;
}