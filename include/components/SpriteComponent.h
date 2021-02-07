#ifndef SPRITECOMPONENT_H_
#define SPRITECOMPONENT_H_

#include <fundamentals/AssetManager.h>
#include "TransformComponent.h"
#include "Animation.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;

    bool animated{false};
    int numFrames{0};
    int animationSpeed{0};
    bool fixed{false};
    std::map<std::string, Animation> animations;
    std::string currentAnimationName;
    unsigned int animationIndex{0};

public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    void initialize() override
    {
        transform = owner->getComponent<TransformComponent>();
        sourceRectangle.x = 0;
        sourceRectangle.y = 0;
        sourceRectangle.w = transform->width;
        sourceRectangle.h = transform->height;
    }

    void update(float) override
    {
        if (animated)
        {
            sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
        }
        sourceRectangle.y = animationIndex * transform->height;

        destinationRectangle.x = static_cast<int>(transform->position.x) - (fixed ? 0:Game::camera.x);
        destinationRectangle.y = static_cast<int>(transform->position.y) - (fixed ? 0:Game::camera.y);
        destinationRectangle.w = transform->width * transform->scale;
        destinationRectangle.h = transform->height * transform->scale;
    }

    virtual void render() override
    {
        TextureManager::draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
    }

    SpriteComponent(const char *assetTextureId)
    {
        animated = false;
        fixed = false;
        setTexture(assetTextureId);
    }

    SpriteComponent(const char *assetTextureId, int numFrames, int animationSpeed, bool hasDirections, bool fixed)
    {
        animated = true;
        this->numFrames = numFrames;
        this->animationSpeed = animationSpeed;
        this->fixed = fixed;

        if (hasDirections)
        {
            Animation downAnimation = Animation(0, numFrames, animationSpeed);
            Animation rightAnimation = Animation(1, numFrames, animationSpeed);
            Animation leftAnimation = Animation(2, numFrames, animationSpeed);
            Animation upAnimation = Animation(3, numFrames, animationSpeed);

            animations.emplace("downAnimation", downAnimation);
            animations.emplace("rightAnimation", rightAnimation);
            animations.emplace("leftAnimation", leftAnimation);
            animations.emplace("upAnimation", upAnimation);

            this->animationIndex = 0;
            this->currentAnimationName = "downAnimation";
        }
        else
        {
            Animation singleAnimation = Animation(0, numFrames, animationSpeed);
            animations.emplace("singleAnimation", singleAnimation);

            this->animationIndex = 0;
            this->currentAnimationName = "singleAnimation";
        }

        play(this->currentAnimationName);

        setTexture(assetTextureId);
    }

    void play(std::string animationName)
    {
        numFrames = animations[animationName].numFrames;
        animationIndex = animations[animationName].index;
        animationSpeed = animations[animationName].animationSpeed;
        currentAnimationName = animationName;
    }

    void setTexture(std::string assetTextureId)
    {
        texture = Game::assetManager->getTexture(assetTextureId);
    }
};

#endif