#include <fundamentals/Game.h>
#include <fundamentals/AssetManager.h>
#include <fundamentals/Map.h>

#include <components/TransformComponent.h>
#include <components/SpriteComponent.h>
#include <components/KeyboardControlComponent.h>
#include <components/ColliderComponent.h>
#include <components/TextLabelComponent.h>
#include <components/ProjectileEmitterComponent.h>

EntityManager manager;
AssetManager *Game::assetManager = new AssetManager(&manager);
SDL_Renderer *Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map *Game::map;
bool Game::debug = false;

int prev;

Entity *mainPlayer;

Game::Game()
{
    this->running = false;
}

Game::~Game()
{
}

void Game::initialize(int width, int height)
{
    //If everthing is OK and initialization successful function returns 0 (SDL_Init)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "Error initializing SDL TTF" << std::endl;
    }

    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!window)
    {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }

    //Index -1 default driver
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    loadLevel(1);

    running = true;
}

void Game::loadLevel(int levelNumber)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);

    lua.script_file("./assets/scripts/" + levelName + ".lua");

    sol::table levelData = lua[levelName];

    //LOAD ASSETS
    sol::table levelAssets = levelData["assets"];
    unsigned int assetIndex = 0;
    while (true)
    {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];

            if (assetType.compare("texture") == 0)
            {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->addTexture(assetId, assetFile.c_str());
            }
            if (assetType.compare("font") == 0)
            {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->addFont(assetId, assetFile.c_str(), fontSize);
            }
        }
        assetIndex++;
    }

    //LOAD MAP AND TILE INFO
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];
    map = new Map(mapTextureId, static_cast<int>(levelMap["scale"]), static_cast<int>(levelMap["tileSize"]));
    map->loadMap(mapFile, static_cast<int>(levelMap["mapSizeX"]), static_cast<int>(levelMap["mapSizeY"]));

    //LOAD ENTITIES
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;

    while (true)
    {
        sol::optional<sol::table> existsEntityNode = levelEntities[entityIndex];

        if (existsEntityNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            //Add new entity
            auto &newEntity(manager.addEntity(entityName, entityLayerType));

            //Add transform component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if (existsTransformComponent != sol::nullopt)
            {
                newEntity.addComponent<TransformComponent>(
                static_cast<int>(entity["components"]["transform"]["position"]["x"]), 
                static_cast<int>(entity["components"]["transform"]["position"]["y"]), 
                static_cast<int>(entity["components"]["transform"]["velocity"]["x"]), 
                static_cast<int>(entity["components"]["transform"]["velocity"]["y"]), 
                static_cast<int>(entity["components"]["transform"]["width"]), 
                static_cast<int>(entity["components"]["transform"]["height"]), 
                static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }

            //Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt)
            {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool animated = entity["components"]["sprite"]["animated"];
                if(animated){
                    newEntity.addComponent<SpriteComponent>(
                    textureId.c_str(),
                    static_cast<int>(entity["components"]["sprite"]["frameCount"]), 
                    static_cast<int>(entity["components"]["sprite"]["animationSpeed"]), 
                    static_cast<bool>(entity["components"]["sprite"]["hasDirections"]), 
                    static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                }else
                {
                    std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                    newEntity.addComponent<SpriteComponent>(textureId.c_str());
                }

            }

            //Add collider component
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if(existsColliderComponent != sol::nullopt){
                std::string colliderTag = entity["components"]["collider"]["tag"];
                newEntity.addComponent<ColliderComponent>(
                colliderTag,
                static_cast<int>(entity["components"]["transform"]["position"]["x"]), 
                static_cast<int>(entity["components"]["transform"]["position"]["y"]), 
                static_cast<int>(entity["components"]["transform"]["width"]), 
                static_cast<int>(entity["components"]["transform"]["height"]) 
                );
            }

            //Add input control component
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if(existsInputComponent != sol::nullopt){
                sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                if(existsKeyboardInputComponent != sol::nullopt){
                    std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                    std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                    std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                    std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                    std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                    newEntity.addComponent<KeyboardControlComponent>(upKey, downKey, rightKey, leftKey, shootKey);
                }
            }
            
            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                int projectileRange = entity["components"]["projectileEmitter"]["range"];
                int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                Entity& projectile(manager.addEntity("projectile", PROJECTILE_LAYER));
                projectile.addComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.addComponent<SpriteComponent>(textureAssetId.c_str());
                projectile.addComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.addComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }
        }
        entityIndex++;
    }

    mainPlayer = manager.getEntityByName("player");
}

bool Game::isRunning() const
{
    return running;
}

void Game::processInput()
{

    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        running = false;
        break;

    case SDL_KEYDOWN:

        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
        }
        else if (prev != SDLK_F1 && event.key.keysym.sym == SDLK_F1)
        {
            Game::debug = !debug;
            std::cout << "Debug ON" << std::endl;
            prev = SDLK_F1;
        }
        break;

    case SDL_KEYUP:
        prev = 0;
        break;
    default:
        break;
    }
}

void Game::update()
{
    //Delta time is the difference is the ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    //Clamp deltaTime to a maximum value to be able to prevent time travels
    deltaTime = (deltaTime > 0.05) ? 0.05 : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    manager.update(deltaTime);

    checkCollisions();

    handleCameraMovement();
}

void Game::render()
{

    /*
    * Clear the back buffer
    * Draw all game objects
    * Swap front and back buffers
    */

    //Wait until 16ms has elapsed since the laBst frame
    uint32_t timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(timeToWait);
    }

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    if (manager.hasNoEntities())
    {
        return;
    }
    else
    {
        manager.render();
    }

    SDL_RenderPresent(renderer);
}

void Game::handleCameraMovement()
{
    TransformComponent *mainPlayerTransform = mainPlayer->getComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::checkCollisions()
{
    //FEATURE WORK
}

void Game::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}