#include <fundamentals/FontManager.h>

TTF_Font* FontManager::loadFont(const char* filePath, int fontSize){
    return TTF_OpenFont(filePath, fontSize);
}

void FontManager::draw(SDL_Texture* texture, SDL_Rect position){
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}