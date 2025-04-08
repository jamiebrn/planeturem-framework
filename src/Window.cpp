#include "Window.hpp"

bool pl::Window::glInitialised = false;

pl::Window::Window()
{
    initialised = false;
    window = nullptr;
    glContext = nullptr;
    updateFunction = nullptr;
}

pl::Window::Window(const std::string& title, int width, int height, uint32_t flags)
{
    initialised = false;
    updateFunction = nullptr;
    create(title, width, height, flags);
}

pl::Window::~Window()
{
    SDL_DestroyWindow(window);
    SDL_DelEventWatch(eventWatch, this);
}

void pl::Window::create(const std::string& title, int width, int height, uint32_t flags)
{
    if (initialised)
    {
        SDL_DestroyWindow(window);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | flags);
    
    if (!initialised)
    {
        glContext = SDL_GL_CreateContext(window);
        SDL_AddEventWatch(eventWatch, this);
        initialised = true;
    }

    SDL_GL_MakeCurrent(window, glContext);

    if (!glInitialised)
    {
        gladLoaderLoadGL();
        glInitialised = true;
        glEnable(GL_BLEND);
    }

    nonFullscreenWidth = width;
    nonFullscreenHeight = height;

    running = true;
}

void pl::Window::toggleFullscreen()
{
    uint32_t windowFlags = (SDL_GetWindowFlags(window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
    const char* title = SDL_GetWindowTitle(window);
    create(std::string(title), nonFullscreenWidth, nonFullscreenHeight, windowFlags);
}

void pl::Window::setVSync(bool enabled)
{
    SDL_GL_SetSwapInterval(enabled);
}

void pl::Window::setIcon(const Image& image)
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(image.getPixelPtr(), image.getWidth(), image.getHeight(), 32, image.getWidth() * 4, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);
}

void pl::Window::setUpdateFunction(void (*updateFunction)(void*), void* updateFunctionData)
{
    this->updateFunction = updateFunction;
    this->updateFunctionData = updateFunctionData;
}

void pl::Window::startUpdate()
{
    if (!updateFunction)
    {
        printf("ERROR: Attempted to start window update with null update function\n");
        return;
    }

    while (isOpen())
    {
        (*updateFunction)(updateFunctionData);
    }
}

bool pl::Window::isOpen()
{
    return running;
}

void pl::Window::close()
{
    running = false;
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

    if (event.type == SDL_QUIT)
    {
        running = false;
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

SDL_Window* pl::Window::getSDLWindow()
{
    return window;
}

void pl::Window::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, getWidth(), getHeight());
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

int pl::Window::eventWatch(void* data, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT)
    {
        if (event->window.event == SDL_WINDOWEVENT_EXPOSED)
        {
            Window* window = (Window*)data;
            window->bind();

            if (window->updateFunction)
            {
                (*window->updateFunction)(window->updateFunctionData);
            }
        }
    }

    return 1;
}