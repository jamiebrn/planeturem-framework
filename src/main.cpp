#include <gl.h>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <chrono>
#include <string>

#include "Vertex.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    srand(time(NULL));

    SDL_Window* window = SDL_CreateWindow("GL-Framework", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, glContext);
    
    gladLoaderLoadGL();

    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

    std::vector<Vertex> vertexData = {
        {{-0.5, -0.5}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5, -0.5}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5, 0.5}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{0.5, -0.5}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5, 0.5}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5, 0.5}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    Shader shader;
    shader.load("Data/default.vert", "Data/default.frag");

    Texture texture;
    texture.loadTexture("Data/icon.png");

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f) + sizeof(Color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

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
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {

            }
        }
        
        xPos += (keyboardState[SDL_SCANCODE_D] - keyboardState[SDL_SCANCODE_A]) * dt;
        yPos -= (keyboardState[SDL_SCANCODE_S] - keyboardState[SDL_SCANCODE_W]) * dt;

        vertexData = {
            {{-0.5f + xPos, -0.5f + yPos}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.5f + xPos, -0.5f + yPos}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{-0.5f + xPos, 0.5f + yPos}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
            {{0.5f + xPos, -0.5f + yPos}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.5f + xPos, 0.5f + yPos}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f + xPos, 0.5f + yPos}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
        };

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertexArray);
        shader.use();
        texture.use();
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);

        SDL_SetWindowTitle(window, ("GL-Framework - " + std::to_string((int(1.0 / dt))) + "fps").c_str());
    }
    
    SDL_DestroyWindow(window);

    return 0;
}