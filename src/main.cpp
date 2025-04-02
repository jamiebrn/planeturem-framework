#include <vector>
#include <chrono>
#include <string>

#include "Engine.hpp"
#include "Window.hpp"
#include "Vertex.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "SpriteBatch.hpp"

pl::Shader shader;
pl::Texture texture;
std::chrono::high_resolution_clock timeClock;
std::vector<pl::Vector2f> positions;
auto nowTime = timeClock.now();
auto lastTime = nowTime;
float gameTime = 0;
pl::Window window;
const uint8_t* keyboardState;

void update(void* data)
{
    lastTime = nowTime;
    nowTime = timeClock.now();
    float dt = std::chrono::duration_cast<std::chrono::microseconds>(nowTime - lastTime).count() / 1000000.0f;
    gameTime += dt;

    SDL_Event event;
    while (window.pollEvent(event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_F11)
            {
                window.toggleFullscreen();
            }
        }
    }

    window.clear({0, 0, 0, 255});

    pl::SpriteBatch spriteBatch;
    spriteBatch.beginDrawing();

    for (int i = 0; i < positions.size(); i++)
    {
        pl::DrawData drawData;
        drawData.shader = &shader;
        drawData.texture = &texture;
        drawData.position = positions[i];
        drawData.scale = pl::Vector2f(0.5, 0.5);
        drawData.textureRect = {0, 0, 256, 256};
        drawData.centerRatio = pl::Vector2f(0.5, 0.5);
        drawData.rotation = (gameTime + i) * 30;
        spriteBatch.draw(window, drawData);
    }

    spriteBatch.endDrawing(window);

    window.swapBuffers();

    window.setTitle("GL-Framework - " + std::to_string(int(1.0 / dt)) + "fps");
}

int main(int argc, char* argv[])
{
    pl::init();

    window.create("GL-Framework", 800, 600, SDL_WINDOW_RESIZABLE);

    Image image;
    image.loadFromFile("Data/icon.png");
    window.setIcon(image);

    keyboardState = SDL_GetKeyboardState(NULL);

    shader.load("Data/default.vert", "Data/default.frag");

    texture.loadTexture("Data/icon.png");

    for (int i = 0; i < 1000; i++)
    {
        positions.push_back(pl::Vector2f(rand() % 800, rand() % 600));
    }

    window.setUpdateFunction((*update), nullptr);
    window.startUpdate();

    pl::deinit();

    return 0;
}