#pragma once

#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "util/definitions.h"
#include "render/camera.h"

class Texture {
protected:
    SDL_Texture* texture_;
    SDL_Renderer* renderer_;
    Camera* camera;

public:
    Texture();
    ~Texture();
    bool Initialize(SDL_Renderer*, Camera*, std::string);
    void Render(Vector2, float32, SDL_Rect);
    // void Render(Vector2, float32);
    void GetDimensions(int32*, int32*);
};
