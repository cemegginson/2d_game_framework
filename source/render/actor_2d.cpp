#include "render/actor_2d.h"

#include <map>
#include <memory>
#include <vector>
#include <string>

Actor2D::Actor2D() {}

Actor2D::~Actor2D() {
    while (components_.size() > 0) {
        delete components_.back();
        components_.pop_back();
    }
}

void Actor2D::Initialize(std::string name,
                       Vector2 transform,
                       uint32 angle,
                       bool controllable) {
    name_ = name;
    transform_ = transform;
    angle_ = angle;
    sprite_clip_.x = 0;
    sprite_clip_.y = 0;
    sprite_clip_.w = 35;
    sprite_clip_.h = 60;

    actor_events_.insert(std::pair<ActorEvent, bool>(kMoveUp, false));
    actor_events_.insert(std::pair<ActorEvent, bool>(kMoveDown, false));
    actor_events_.insert(std::pair<ActorEvent, bool>(kMoveLeft, false));
    actor_events_.insert(std::pair<ActorEvent, bool>(kMoveRight, false));
    actor_events_.insert(std::pair<ActorEvent, bool>(kTurnLeft, false));
    actor_events_.insert(std::pair<ActorEvent, bool>(kTurnRight, false));

    controllable_ = controllable;
}

void Actor2D::AddComponent(Component* component) {
    components_.push_back(component);
}

void Actor2D::Update(float32 delta_time) {
    UNUSED(delta_time);  // make linter shutup
}

Vector2 Actor2D::position() {
    return position_;
}
void Actor2D::set_position(Vector2 new_position) {
    position_ = new_position;
}

float32 Actor2D::angle() {
    return angle_;
}
void Actor2D::set_angle(float32 new_angle) {
    angle_ = new_angle;
}

SDL_Rect Actor2D::sprite_clip() {
    return sprite_clip_;
}
void Actor2D::set_sprite_clip(SDL_Rect new_sprite_clip) {
    sprite_clip_ = new_sprite_clip;
}

void Actor2D::SetEvent(ActorEvent event, bool value) {
    actor_events_.at(event) = value;
}
bool Actor2D::CheckEvent(ActorEvent event) {
    return actor_events_.at(event);
}
void Actor2D::ResetEvents() {
    for (auto iter = actor_events_.begin(); iter != actor_events_.end(); ++iter) {
        iter->second = false;
    }
}

bool Actor2D::IsControllable() {
    return controllable_;
}
