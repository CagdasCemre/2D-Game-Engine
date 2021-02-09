#ifndef TRANSFORMCOMPONENT_H_
#define TRANSFORMCOMPONENT_H_

#include <fundamentals/Game.h>
#include <fundamentals/Map.h>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class TransformComponent: public Component
{

public:
    glm::vec2 position;
    glm::vec2 velocity;
    int width{0};
    int height{0};
    int scale{0};

private:
   
public:
    TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s){
        position = glm::vec2(posX, posY);
        velocity = glm::vec2(velX, velY);
        width = w;
        height = h;
        scale = s;
    }
    
    void initialize() override{

    }
    
    void update(float deltaTime) override{
        //update the position/velocity as a function of deltaTime
        int mapScale = Game::map->getScale();

        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;

        position.x = position.x < 0 ? 0 : position.x;
        position.y = position.y < 0 ? 0 : position.y;
        position.x = position.x > (Game::camera.w * mapScale) - width ? (Game::camera.w * mapScale) - width : position.x;
        position.y = position.y > (Game::camera.h * mapScale ) - height ? (Game::camera.h * mapScale ) - height : position.y;
    }
    
    void render() override{
    }

};

#endif


