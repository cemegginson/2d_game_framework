#pragma once

#include <memory>

#include "virtual/component.h"
#include "render/renderer.h"
#include "render/texture.h"

class Renderer;

class Sprite : public Component {
protected:
    Texture* texture_;
    Renderer* renderer_;
    SDL_Rect sprite_clip_;

public:
    Sprite(Actor*);
    ~Sprite();
    void Initialize(Renderer*, Texture*);
    void Update(std::shared_ptr<void> delta_time);
    void Draw();
};
