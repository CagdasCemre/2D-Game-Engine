#ifndef GAME_H_
#define GAME_H_

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <glm/glm.hpp>

#include "Constants.h"
#include "EntityManager.h"

class AssetManager;
class Map;

class Game
{

public:
    static bool debug;

private:
    int ticksLastFrame{0};

    bool running{false};

    SDL_Window *window{nullptr};

public:
    Game();
    virtual ~Game();

    static SDL_Renderer *renderer;
    static AssetManager *assetManager;
    static SDL_Event event;
    static SDL_Rect camera;
    static Map *map;

    void handleCameraMovement();
    void loadLevel(int levelNumber);
    void checkCollisions();

    bool isRunning() const;

    void initialize(int width, int height);
    void processInput();
    void update();
    void render();
    void destroy();
};

#endif