#pragma once

#include <memory>

#include "virtual/component.h"
#include "render/sdl_renderer.h"
#include "render/sdl_texture.h"

class SDLRenderer;

class Sprite : public Component {
protected:
    SDLTexture* texture_;
    SDLRenderer* renderer_;
    SDL_Rect sprite_clip_;

public:
    Sprite(Actor*);
    ~Sprite();
    void Initialize(Renderer*, SDLTexture*);
    void Update(std::shared_ptr<void> delta_time);
    void Draw();
};
