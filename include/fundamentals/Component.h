#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "fundamentals/Entity.h"

class Entity; // Forward Decleration

class Component
{

public:
    Entity* owner{nullptr};

private:
    
public:
    Component(/* args */){};
    virtual ~Component(){};
    virtual void initialize()=0;
    virtual void update(float deltaTime)=0;
    virtual void render()=0;
};


#endif