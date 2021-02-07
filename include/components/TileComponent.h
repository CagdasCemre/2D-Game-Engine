#ifndef TILECOMPOENENT_H_
#define TILECOMPOENENT_H_

#include <fundamentals/EntityManager.h>
#include <fundamentals/AssetManager.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class TileComponent: public Component
{
public:
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    glm::vec2 position;

public:
    TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId){
        texture = Game::assetManager->getTexture(assetTextureId);

        sourceRectangle.x = sourceRectX;
        sourceRectangle.y = sourceRectY;
        sourceRectangle.w = tileSize;
        sourceRectangle.h = tileSize;


        destinationRectangle.x = x;
        destinationRectangle.y = y;
        destinationRectangle.w = tileSize * tileScale;
        destinationRectangle.h = tileSize * tileScale;

        position.x = x;
        position.y = y;
    }

    ~TileComponent(){
        SDL_DestroyTexture(texture);
    }

    void update(float deltaTime) override{

    }

    void render() override{
        TextureManager::draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
    }

    virtual void initialize(){
        
    }

};

#endif