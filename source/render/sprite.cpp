// Copyright 2015 Casey Megginson and Blaise Koch
#include "render/sprite.h"
#include "event_system/Subscriber.h"

Sprite::Sprite(Actor2D* owner) {
    texture_ = nullptr;

    Subscriber* s = new Subscriber(this);
    s->method = std::bind(&Sprite::Update, this, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(s, "EVENT_COMPONENT_UPDATE");
    subscribers.push_back(s);
}

Sprite::~Sprite() {
    while (subscribers.size() > 0) {
        delete subscribers.back();
        subscribers.pop_back();
    }
}

void Sprite::Initialize(Renderer* renderer, Texture* texture) {
    renderer_ = renderer;
    reinterpret_cast<Renderer*>(renderer_)->AddSprite(this);
    texture_ = texture;

    // Add Sprite to renderer_
    // renderer_->AddSprite(shared_from_this());
}

void Sprite::Update(std::shared_ptr<void> delta_time) {
    UNUSED(delta_time);
    sprite_clip_ = owner_->sprite_clip();
}

void Sprite::Draw() {
    Vector2 position = owner_->position();
    float32 angle = owner_->angle();
    // Not finished here
    texture_->Render(position, angle, sprite_clip_);
    // texture_->Render(position, angle);
}
