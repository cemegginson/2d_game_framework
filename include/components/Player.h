#pragma once

#include <list>
#include <string>
#include <vector>

#include "art_library/ArtAssetLibrary.h"
#include "components/Bullet.h"
#include "render/actor_2d.h"
#include "virtual/component.h"
#include "util/definitions.h"


class Player : public Component {
protected:
    Actor2D* owner_;
    std::vector<Subscriber*> subscribers_;
    ArtAssetLibrary* art_library_;
    // Renderer* renderer_;
    InputDevice* input_device_;
    uint32 travel_;
    uint32 radius_;
    Vector2 center_;
    float32 rotation_;
    float32 last_fire_time_;

public:
    Player(Actor2D*);
    ~Player();
    void Update(std::shared_ptr<void> delta_time);
    void Initialize();

    void set_input_device(InputDevice*);
};
