#ifndef KEYBOARDCONTROLCOMPONENT_H_
#define KEYBOARDCONTROLCOMPONENT_H_

#include <fundamentals/Game.h>
#include <fundamentals/EntityManager.h>

#include "TransformComponent.h"
#include "SpriteComponent.h"

#include <iostream>

class KeyboardControlComponent:public Component
{
public:
    std::string upKey;
    std::string downKey;
    std::string rightKey;
    std::string leftKey;
    std::string shootKey;
    TransformComponent* transform{nullptr};
    SpriteComponent* sprite{nullptr};
    
public:
    KeyboardControlComponent();
    KeyboardControlComponent(std::string upKey, std::string downKey, std::string rightKey, std::string leftKey, std::string shootKey){
        
        this->upKey = getSDLKeyStringCode(upKey);
        this->downKey = getSDLKeyStringCode(downKey);
        this->rightKey = getSDLKeyStringCode(rightKey);
        this->leftKey = getSDLKeyStringCode(leftKey);
        this->shootKey = getSDLKeyStringCode(shootKey);
    }

    std::string getSDLKeyStringCode(std::string key){
        if(key.compare("up") == 0) return "1073741906";
        if(key.compare("down") == 0) return "1073741905";
        if(key.compare("left") == 0) return "1073741904";
        if(key.compare("right") == 0) return "1073741903";
        if(key.compare("space") == 0) return "32";

        return std::to_string(static_cast<int>(key[0]));
    }

    void initialize()override{ 
        transform = owner->getComponent<TransformComponent>();
        sprite = owner->getComponent<SpriteComponent>();
    }

    void update(float deltaTime) override{
        if(Game::event.type == SDL_KEYDOWN){
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            
            if(keyCode.compare(upKey) == 0){
                transform->velocity.y = -200;
                transform->velocity.x = 0;

                sprite->play("upAnimation");
            }
            if(keyCode.compare(rightKey) == 0){
                transform->velocity.y = 0;
                transform->velocity.x = 200;

                sprite->play("rightAnimation");
            }
            if(keyCode.compare(leftKey) == 0){
                transform->velocity.y = 0;
                transform->velocity.x = -200;

                sprite->play("leftAnimation");
            }
            if(keyCode.compare(downKey) == 0){
                transform->velocity.y = 200;
                transform->velocity.x = 0;

                sprite->play("downAnimation");
            }
            if(keyCode.compare(shootKey) == 0){
                //Shoot projectiles
            }

        }
        if(Game::event.type == SDL_KEYUP){
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            if(keyCode.compare(upKey) == 0){
                transform->velocity.y = 0;

            }
            if(keyCode.compare(rightKey) == 0){
                transform->velocity.x = 0;

            }
            if(keyCode.compare(leftKey) == 0){
                transform->velocity.x = 0;

            }
            if(keyCode.compare(downKey) == 0){
                transform->velocity.y = 0;

            }
            if(keyCode.compare(shootKey) == 0){
                //stop shooting
            }
        }

    }

    void render(){

    }


};

#endif