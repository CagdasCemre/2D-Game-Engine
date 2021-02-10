#ifndef PROJECTILEEMITTERCOMPONENT_H_
#define PROJECTILEEMITTERCOMPONENT_H_

#include <glm/glm.hpp>
#include <fundamentals/EntityManager.h>

#include "TransformComponent.h"

class ProjectileEmitterComponent: public Component
{
private:
    TransformComponent* transform;
    glm::vec2 origin;
    int speed;
    int range;
    float angleRad;
    bool shootLoop;

public:
    ProjectileEmitterComponent(int speed, int angleDeg, int range, bool shootLoop):speed(speed), range(range), shootLoop(shootLoop){
        this->angleRad = glm::radians(static_cast<float>(angleDeg));
    }

    void initialize() override{
        transform = owner->getComponent<TransformComponent>();
        origin = glm::vec2(transform->position.x, transform->position.y);
        transform->velocity = glm::vec2(speed * glm::cos(angleRad), speed * glm::sin(angleRad)); 
    }

    void update(float deltaTime) override{
        if(glm::distance(transform->position, origin) > range){
            if(shootLoop){
                transform->position.x = origin.x;
                transform->position.y = origin.y;
            }else{
                owner->destroy();
            }
        }
    }

    void render() override{}
};

#endif