#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>
#include <map>
#include <string>


class Component;// Forward Decleration
class EntityManager;// Forward Decleration

class Entity
{

private:
    EntityManager& manager;
    bool active{false};
    

public:
    std::string name;
    std::vector<Component*> components;
    std::map<const std::type_info*,Component*> componentTypeMap;

public:
    Entity(EntityManager& manager);
    Entity(EntityManager& manager, std::string name);
    virtual ~Entity();

    bool isActive() const;

    void update(float deltaTime);
    void render();
    
    void destroy();

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... args){
        T* newComponent(new T(std::forward<TArgs>(args)...));
        
        newComponent->owner = this;
        components.push_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->initialize();

        return *newComponent;
    }
    
    template<typename T>
    T* getComponent(){
        return static_cast<T*>(componentTypeMap[&typeid(T)]);
    }

    template<typename T>
    bool hasComponent() const{
        return componentTypeMap.count(&typeid(T));
    }

    void listAllComponents();

};

#endif