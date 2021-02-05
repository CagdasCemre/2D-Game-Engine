#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <string>

class SpriteCompoenent;

class Animation
{
public:
    unsigned int index{0};
    unsigned int numFrames{0};
    unsigned int animationSpeed{0};
public:
    Animation();
    Animation(unsigned int index, unsigned int numFrames, unsigned int animationSpeed);
};

#endif