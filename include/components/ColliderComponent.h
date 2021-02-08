#ifndef COLLIDERCOMPONENT_H_
#define COLLIDERCOMPONENT_H_


#include <fundamentals/EntityManager.h>
#include <fundamentals/Game.h>

#include "TransformComponent.h"

class ColliderComponent: public Component
{
public:
    std::string colliderTag;

    SDL_Rect collider;
    SDL_Rect sourceRectangel;
    SDL_Rect destinationRectangle;
    TransformComponent* transform;

public:
    ColliderComponent(std::string colliderTag, int x, int y, int w, int h){
        this->colliderTag = colliderTag;
        this->collider = {x, y, w, h};
    }

    void initialize() override{
        transform = owner->getComponent<TransformComponent>();
        sourceRectangel  = {0, 0, transform->width, transform->height};
        destinationRectangle  = {collider.x, collider.y, collider.w, collider.h};     
    }


    void update(float deltaTime)override{
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;

        destinationRectangle.x = collider.x - Game::camera.x;
        destinationRectangle.y = collider.y - Game::camera.y;
    }

    void render()override{}
};

#endif