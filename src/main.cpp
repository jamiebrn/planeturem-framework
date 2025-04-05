#include <vector>
#include <chrono>
#include <string>

#include "Engine.hpp"
#include "Window.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SpriteBatch.hpp"
#include "Graphics/Framebuffer.hpp"
#include "Graphics/Font.hpp"

int main(int argc, char* argv[])
{
    srand(time(NULL));

    pl::init();

    pl::Window window;
    window.create("GL-Framework", 800, 600, SDL_WINDOW_RESIZABLE);
    window.setVSync(true);

    Image image;
    image.loadFromFile("Data/icon.png");
    window.setIcon(image);

    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

    pl::Shader shader;
    shader.load("Data/default.vert", "Data/default.frag");

    pl::Texture texture;
    texture.loadTexture("Data/icon.png");
    texture.setTextureRepeat(true);
    texture.setLinearFilter(false);

    pl::Font font;
    pl::Shader fontShader;
    font.loadFromFile("Data/upheavtt.ttf");
    fontShader.load("Data/default.vert", "Data/font.frag");

    pl::Framebuffer framebuffer;
    framebuffer.create(800, 600);

    std::vector<pl::Vector2f> positions;
    for (int i = 0; i < 1000; i++)
    {
        positions.push_back(pl::Vector2f(rand() % 800, rand() % 600));
    }

    int outlineThickness = 0;

    std::chrono::high_resolution_clock timeClock;
    auto nowTime = timeClock.now();
    auto lastTime = nowTime;
    float gameTime = 0;

    while (window.isOpen())
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

            if (event.type == SDL_MOUSEWHEEL)
            {
                outlineThickness = std::max(outlineThickness + event.wheel.y, 1);
            }
        }

        framebuffer.clear({0, 0, 0, 255});
        
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
            spriteBatch.draw(framebuffer, drawData);
        }
        
        spriteBatch.endDrawing(framebuffer);

        window.clear({0, 0, 0, 255});

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        pl::VertexArray vertexArray;
        vertexArray.addQuad(pl::Rect<float>(0, 0, mouseX, mouseY), pl::Color(1, 1, 1, 1), pl::Rect<float>(0, 0, (float)mouseX / framebuffer.getWidth(), (float)mouseY / framebuffer.getHeight()));
        window.draw(vertexArray, shader, framebuffer.getTexture(), pl::BlendMode::Alpha);

        window.swapBuffers();

        window.setTitle("GL-Framework - " + std::to_string(int(1.0 / dt)) + "fps");   
    }

    pl::deinit();

    return 0;
}