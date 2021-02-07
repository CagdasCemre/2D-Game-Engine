#ifndef MAP_H_
#define MAP_H_

#include <string>

class Map
{
private:
    std::string textureId{""};
    int scale{0};
    int tileSize{0};

public:
    Map(std::string textureId, int scale, int tileSize);
    ~Map();

    void loadMap(std::string filePath, int mapSizeX, int mapSizeY);
    void addTile(int sourceX, int sourceY, int x, int y);
};

#endif