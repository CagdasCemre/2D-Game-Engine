#include <fstream>
#include <fundamentals/Map.h>
#include <fundamentals/Game.h>
#include <fundamentals/EntityManager.h>

#include <components/TileComponent.h>

extern EntityManager manager;
Map::Map(std::string textureId, int scale, int tileSize): textureId(textureId), scale(scale), tileSize(tileSize)
{
}
Map::~Map()
{
}

void Map::loadMap(std::string filePath, int mapSizeX, int mapSizeY)
{
    std::fstream mapFile;
    mapFile.open(filePath);

    for(int y = 0 ; y < mapSizeY ; y++){
        for(int x = 0 ; x < mapSizeX ; x++){
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize;

            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;

            addTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));

            mapFile.ignore(); // Bypass comma
        }
    }
    mapFile.close();
}

void Map::addTile(int sourceRectX, int sourceRectY, int x, int y)
{
    Entity& newTile(manager.addEntity("Tile", LayerType::TILEMAP_LAYER));
    newTile.addComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize,scale, textureId);
}

int Map::getScale(){
    return scale;
}