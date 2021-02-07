#include "fundamentals/EntityManager.h"

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