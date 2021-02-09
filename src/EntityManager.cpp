#include "fundamentals/EntityManager.h"
#include "fundamentals/Collision.h"
#include <components/ColliderComponent.h>

#include <iostream>

EntityManager::EntityManager(/* args */)
{
}

EntityManager::~EntityManager()
{
}

void EntityManager::clearData(){
    for(auto entity: entities){
        entity->destroy();
    }
}

bool EntityManager::hasNoEntities() const{
    return entities.size() == 0;
}

void EntityManager::update(float deltaTime){
    for(auto& entity: entities){
        entity->update(deltaTime);
    }
}

void EntityManager::render(){
    for(int layerNum = 0 ; layerNum < NUM_LAYERS ; layerNum++){
        for(auto& entity : getEntitiesByLayer(static_cast<LayerType>(layerNum))){
            entity->render();
        }
    }
}

Entity& EntityManager::addEntity(std::string entityName, LayerType layer){
    Entity *entity = new Entity(*this, entityName, layer);
    entities.push_back(entity);
    
    return *entity;
}

std::vector<Entity*> EntityManager::getEntities() const{
    return entities;
}

std::vector<Entity*> EntityManager::getEntitiesByLayer(LayerType layer) const{
    std::vector<Entity*> temp;
    for(auto& entity : entities){
        if(entity->layer == layer){
            temp.push_back(entity);
        }
    }
    return temp;
}

unsigned int EntityManager::getEntityCount(){
    return entities.size();
}

void EntityManager::listAllEntities() const{
    for(Entity* e: entities){
        std::cout << "Entity Name: " << e->name << std::endl;
        e->listAllComponents();
    }
}

std::string EntityManager::checkEntityCollisions(Entity& myEntity) const{
    ColliderComponent* myCollider = myEntity.getComponent<ColliderComponent>();
    for(auto& entity : entities){
        if(entity->name.compare(myEntity.name) == 0 || entity->name.compare("Tile") == 0) 
            continue;
        if(entity->hasComponent<ColliderComponent>()){
            ColliderComponent* otherCollider = entity->getComponent<ColliderComponent>();
            if(Collision::checkRectCollision(myCollider->collider, otherCollider->collider)){
                return otherCollider->colliderTag;
            }
        }
    }
    return std::string();
}