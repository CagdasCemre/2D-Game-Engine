#ifndef COLLISION_H_
#define COLLISION_H_

#include <SDL2/SDL.h>

class Collision
{
   
public:
    static bool checkRectCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB);
};

#endif