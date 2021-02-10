#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include <map>
#include <string>

#include <SDL2/SDL_ttf.h>

#include "fundamentals/EntityManager.h"
#include "fundamentals/TextureManager.h"
#include "fundamentals/FontManager.h"

class AssetManager
{
private:
    EntityManager* manager;
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;

public:
    AssetManager(EntityManager* manager);
    ~AssetManager();

    void clearData();
    void addTexture(std::string textureId, const char* filePath);
    void addFont(std::string fontId, const char* filePath, int fontSize);

    SDL_Texture* getTexture(std::string textureId);
    TTF_Font* getFont(std::string fontId);
};


#endif