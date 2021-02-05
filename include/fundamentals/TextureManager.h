#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "fundamentals/Game.h"
#include <SDL2/SDL_image.h>

class TextureManager
{
private:
    /* data */
public:
    TextureManager(/* args */);
    ~TextureManager();

    static SDL_Texture* loadTexture(const char* fileName);
    static void draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip);
};

#endif