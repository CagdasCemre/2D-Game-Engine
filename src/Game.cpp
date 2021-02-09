#include <fundamentals/Game.h>
#include <fundamentals/AssetManager.h>
#include <fundamentals/Map.h>

#include <components/TransformComponent.h>
#include <components/SpriteComponent.h>
#include <components/KeyboardControlComponent.h>
#include <components/ColliderComponent.h>


EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* Game::map;
bool Game::debug = false;

int prev;

Entity& chopperEntity = manager.addEntity("chopper", LayerType::PLAYER_LAYER);

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
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }

    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if(!window){
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }

    //Index -1 default driver
    renderer = SDL_CreateRenderer(window, -1, 0);

    if(!renderer){
        std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    loadLevel(0);

    running = true;
}


void Game::loadLevel(int levelNumber){
    
    
    assetManager->addTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->addTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->addTexture("radar-image", std::string("./assets/images/radar.png").c_str());

    assetManager->addTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
    map = new Map("jungle-tiletexture", 2, 32);
    map->loadMap("./assets/tilemaps/jungle.map", 25, 20);

    
    Entity& tankEntity = manager.addEntity("tank", LayerType::ENEMY_LAYER);
    tankEntity.addComponent<TransformComponent>(0, 0, 0, 0, 32, 32, 1);
    tankEntity.addComponent<SpriteComponent>("tank-image");
    tankEntity.addComponent<ColliderComponent>("enemy", 0, 0, 32, 32);
    tankEntity.addComponent<KeyboardControlComponent>("up", "down", "right", "left", "space");
    
    chopperEntity.addComponent<TransformComponent>(WINDOW_WIDTH/2, 0, 0, 0, 32, 32, 1);
    chopperEntity.addComponent<SpriteComponent>("chopper-image",2 ,90, true, false);
    chopperEntity.addComponent<KeyboardControlComponent>("w", "s", "d", "a", "space");
    chopperEntity.addComponent<ColliderComponent>("player", WINDOW_WIDTH/2, 0, 32, 32);
    

    Entity& radarEntity = manager.addEntity("Radar", LayerType::UI_LAYER);
    radarEntity.addComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.addComponent<SpriteComponent>("radar-image", 8 ,150, false, true);
    


    //manager.listAllEntities();
}

bool Game::isRunning() const
{
    return running;
}

void Game::processInput(){   

    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        running = false;
        break;
    
    case SDL_KEYDOWN:

        if(event.key.keysym.sym == SDLK_ESCAPE){
            running = false;
        }else if(prev != SDLK_F1 && event.key.keysym.sym == SDLK_F1){
            Game::debug = !debug;
            std::cout << prev << " " << SDLK_F1 << std::endl;
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

void Game::update(){ 
    
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


void Game::render(){   

    /*
    * Clear the back buffer
    * Draw all game objects
    * Swap front and back buffers
    */

    //Wait until 16ms has elapsed since the laBst frame
    uint32_t timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME){
        SDL_Delay(timeToWait);
    }

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    if(manager.hasNoEntities()){
        return;
    }else{
        manager.render();
    }
    

    SDL_RenderPresent(renderer);
}

void Game::handleCameraMovement(){
    TransformComponent* mainPlayerTransform = chopperEntity.getComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH/2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT/2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::checkCollisions(){
    std::string collisionTagType = manager.checkEntityCollisions(chopperEntity);
    if(collisionTagType.compare("enemy") == 0){
        //TODO: do something
        running = false;
    }
}

void Game::destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}