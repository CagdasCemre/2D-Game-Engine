#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include <map>
#include <string>

#include "fundamentals/EntityManager.h"
#include "fundamentals/TextureManager.h"

class AssetManager
{
private:
    EntityManager* manager;
    std::map<std::string, SDL_Texture*> textures; 

public:
    AssetManager(EntityManager* manager);
    ~AssetManager();

    void clearData();
    void addTexture(std::string textureId, const char* filePath);
    SDL_Texture* getTexture(std::string textureId);
};


#endif