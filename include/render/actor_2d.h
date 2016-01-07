#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "SDL.h"

#include "virtual/component.h"

#include "util/definitions.h"
#include "core/input_device.h"
#include "render/sdl_camera.h"

class Component;
// class Actor;

class Actor2D {
protected:
    std::string name_;
    Vector2 position_;
    Vector2 dimensions_;
    float32 angle_;
    std::vector<Component*> components_;
    std::map<ActorEvent, bool> actor_events_;
    bool controllable_;

    // For sprite clipping
    SDL_Rect sprite_clip_;

public:
    Actor2D();
    ~Actor2D();
    void Initialize(std::string, Vector2, uint32, bool);
    void AddComponent(Component*);

    void Update(float32);

    void SetEvent(ActorEvent, bool value = true);
    bool CheckEvent(ActorEvent);
    void ResetEvents();

    bool IsControllable();

    Vector2 position();
    void set_transform(Vector2);

    float32 angle();
    void set_angle(float32);

    SDL_Rect sprite_clip();
    void set_sprite_clip(SDL_Rect);
};
