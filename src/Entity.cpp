#include "fundamentals/Entity.h"
#include "fundamentals/Component.h"

#include <iostream>

Entity::Entity(EntityManager& manager): manager(manager), active(true){
    
}

Entity::Entity(EntityManager& manager, std::string name): manager(manager), name(name), active(true)
{
}

Entity::~Entity()
{
}

void Entity::update(float deltaTime){

    for(auto& component: components){
        component->update(deltaTime);
    }

}

void Entity::render(){
    for(auto& component: components){
        component->render();
    }
}

void Entity::destroy(){
    this->active = false;
}

bool Entity::isActive() const{
    return active;
}

void Entity::listAllComponents(){
    for(std::map<const std::type_info*,Component*>::iterator it = componentTypeMap.begin() ; it != componentTypeMap.end() ; it++){
        std::cout << "  Component<" << (it->first->name()+2) << ">" << std::endl;
    }
     std::cout << std::endl;
}