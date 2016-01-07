#pragma once

#include <vector>
#include <memory>

#include "SDL.h"

#include "util/definitions.h"
#include "render/renderer.h"
#include "render/sprite.h"

class Sprite;

class SDLRenderer {
protected:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    std::vector<Sprite*> sprites_;

    uint32 width_;
    uint32 height_;

public:
    SDLRenderer();
    SDLRenderer(uint32, uint32);
    ~SDLRenderer();
    bool Initialize();
    void AddSprite(Sprite*);

    inline void PreDraw();
    inline void Draw();
    inline void PostDraw();

    RENDER_TYPE type();
    uint32 width();
    uint32 height();
    SDL_Renderer* renderer();
};
