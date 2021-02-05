#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#include "fundamentals/Entity.h"
#include "fundamentals/Component.h"

#include <vector>

class EntityManager
{
private:
    std::vector<Entity*> entities;

public:
    EntityManager(/* args */);
    ~EntityManager();

    void update(float deltaTime);
    void render();

    void clearData();
    bool hasNoEntities() const;
    Entity& addEntity(std::string entityName);
    std::vector<Entity*> getEntities() const;
    unsigned int getEntityCount();

    void listAllEntities() const;
};

#endif