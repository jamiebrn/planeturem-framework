#include <vector>
#include <chrono>
#include <string>

#include "Engine.hpp"
#include "Window.hpp"
#include "Vertex.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "SpriteBatch.hpp"

int main(int argc, char* argv[])
{
    pl::init();

    pl::Window window("GL-Framework", 800, 600, SDL_WINDOW_RESIZABLE);

    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

    pl::Shader shader;
    shader.load("Data/default.vert", "Data/default.frag");

    pl::Texture texture;
    texture.loadTexture("Data/icon.png");

    pl::SpriteBatch spriteBatch;

    std::vector<pl::Vector2f> positions;
    for (int i = 0; i < 1000; i++)
    {
        positions.push_back(pl::Vector2f(rand() % 800, rand() % 600));
    }
    
    float xPos = 0.0f;
    float yPos = 0.0f;

    bool running = true;
    std::chrono::high_resolution_clock clock;
    auto nowTime = clock.now();
    auto lastTime = nowTime;
    float dt = 0;
    float time = 0;

    while (running)
    {
        lastTime = nowTime;
        nowTime = clock.now();
        dt = std::chrono::duration_cast<std::chrono::microseconds>(nowTime - lastTime).count() / 1000000.0f;
        time += dt;

        SDL_Event event;
        while (window.pollEvent(event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        
        xPos += (keyboardState[SDL_SCANCODE_D] - keyboardState[SDL_SCANCODE_A]) * 150 * dt;
        yPos += (keyboardState[SDL_SCANCODE_S] - keyboardState[SDL_SCANCODE_W]) * 150 * dt;

        window.clear({0, 0, 0, 255});

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
            drawData.rotation = (time + i) * 30;
            spriteBatch.draw(window, drawData);
        }

        spriteBatch.endDrawing(window);

        window.swapBuffers();

        window.setTitle("GL-Framework - " + std::to_string(int(1.0 / dt)) + "fps");
    }

    pl::deinit();

    return 0;
}