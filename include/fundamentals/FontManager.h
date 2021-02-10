#ifndef FONTMANAGER_H_
#define FONTMANAGER_H_

#include <fundamentals/Game.h>

class FontManager
{
private:
    /* data */
public:
    static TTF_Font* loadFont(const char* filePath, int fontSize);
    static void draw(SDL_Texture* texture, SDL_Rect position);
};

#endif