#include <iostream>

#include "render/renderer.h"
#include "util/game_utils.h"

Renderer::Renderer() {
    window_ = nullptr;
    renderer_ = nullptr;
    width_ = 0;
    height_ = 0;
}

Renderer::Renderer(uint32 screen_width, uint32 screen_height) {
    window_ = nullptr;
    renderer_ = nullptr;
    width_ = screen_width;
    height_ = screen_height;
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    window_ = nullptr;
    renderer_ = nullptr;
}

bool Renderer::Initialize() {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        LogSDLError(std::cerr, "SDL_Init");
        return false;
    }

    window_ = SDL_CreateWindow("Game Homework 1",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width_,
                               height_,
                               SDL_WINDOW_RESIZABLE
    );

    if (window_ == nullptr) {
        LogSDLError(std::cerr, "SDL_CreateWindow");
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_,
                                   -1,
                                   SDL_RENDERER_ACCELERATED
                                //    | SDL_RENDERER_PRESENTVSYNC //test performance increase
    );

    if (renderer_ == nullptr) {
        LogSDLError(std::cerr, "SDL_CreateRenderer");
        return false;
    }

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);

    return true;
}

inline RENDER_TYPE Renderer::type() {
    return RENDER_TYPE::SDL;
}

uint32 Renderer::width() { return width_; }

uint32 Renderer::height() { return height_; }

SDL_Renderer* Renderer::renderer() {
    return renderer_;
}

void Renderer::AddSprite(Sprite* sprite) {
    sprites_.push_back(sprite);
}

inline void Renderer::PreDraw() {
    SDL_RenderClear(renderer_);
}

inline void Renderer::Draw() {
    // Cycle through every objects' Draw method
    for (auto iter = sprites_.begin(); iter != sprites_.end(); ++iter) {
        (*iter)->Draw();
    }
}

inline void Renderer::PostDraw() {
    SDL_RenderPresent(renderer_);
}
