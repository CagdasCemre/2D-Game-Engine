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
    for(auto& entity: entities){
        entity->render();
    }
}

Entity& EntityManager::addEntity(std::string entityName){
    Entity *entity = new Entity(*this, entityName);
    entities.push_back(entity);
    
    return *entity;
}

std::vector<Entity*> EntityManager::getEntities() const{
    return entities;
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