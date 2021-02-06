#ifndef TRANSFORMCOMPONENT_H_
#define TRANSFORMCOMPONENT_H_

#include "fundamentals/EntityManager.h"
#include "fundamentals/Game.h"

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
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;

        if(position.x > WINDOW_WIDTH-width){
            position.x = WINDOW_WIDTH-width;
        }else if(position.x < 0){
            position.x = 0;
        }
        if(position.y > WINDOW_HEIGHT-height){
            position.y = WINDOW_HEIGHT-height;
        }else if(position.y < 0){
            position.y = 0;
        }
    }
    
    void render() override{
    }

};

#endif


