#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#include "Component.h"

#include <vector>

class EntityManager
{
private:
    std::vector<Entity*> entities;

public:
    EntityManager();
    ~EntityManager();

    void update(float deltaTime);
    void render();

    void clearData();
    bool hasNoEntities() const;
    Entity& addEntity(std::string entityName, LayerType layer);
    Entity* getEntityByName(std::string entityName); // Handle this for collision
    void destroyInactiveEntities();
    std::vector<Entity*> getEntities() const;
    std::vector<Entity*> getEntitiesByLayer(LayerType layer) const;
    std::string checkEntityCollisions(Entity& myEntity) const;
    unsigned int getEntityCount();


    void listAllEntities() const;
};

#endif