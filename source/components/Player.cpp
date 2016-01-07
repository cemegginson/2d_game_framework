// Copyright 2015 Casey Megginson and Blaise Koch
#include <math.h>
#include <memory>
#include "util/box2d_utils.h"
#include "components/Player.h"

Player::Player(Actor2D* owner) {
    owner_ = owner;
    input_device_ = nullptr;
    travel_ = 200;
    rotation_ = 360.0;
    last_fire_time_ = 0;

    Subscriber* s = new Subscriber(this);
    s->method = std::bind(&Player::Update, this, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(s, "EVENT_COMPONENT_UPDATE");
    subscribers_.push_back(s);
}

Player::~Player() {
    while (subscribers_.size() > 0) {
        delete subscribers_.back();
        subscribers_.pop_back();
    }
}

void Player::Update(std::shared_ptr<void> delta_time) {
    float32 time = *reinterpret_cast<float32*>(delta_time.get());
    // float32 angle = owner_->angle();

    // float32 theta = RW2PWAngle(angle - 90);
    // float32 tcos = cos(theta);
    // float32 tsin = sin(theta);

    if (input_device_->IsPressed(kGameA)) {
        owner_->SetEvent(kMoveLeft);
    } else {
        owner_->SetEvent(kMoveLeft, false);
    }
    if (input_device_->IsPressed(kGameD)) {
        owner_->SetEvent(kMoveRight);
    } else {
        owner_->SetEvent(kMoveRight, false);
    }
    if (input_device_->IsPressed(kGameW)) {
        owner_->SetEvent(kMoveUp);
    } else {
        owner_->SetEvent(kMoveUp, false);
    }
    if (input_device_->IsPressed(kGameS)) {
        owner_->SetEvent(kMoveDown);
    } else {
        owner_->SetEvent(kMoveDown, false);
    }

    last_fire_time_ += time;
    // Create bullet if spacebar pressed
    // if(input_device_->IsPressed(kGameSpace) && last_fire_time_ > .075){
    //     last_fire_time_ = 0;
    //     Vector2 direction;
    //     direction.x = 2 * tcos;
    //     direction.y = 2 * tsin;
    //
    //     Vector2 bulletpos;
    //     bulletpos.x = position.x + center.x + radius * tcos;
    //     bulletpos.y = position.y + center.y + radius * tsin;
    //
    //     std::shared_ptr<Bullet> bullet = new Bullet();
    //     bullet->Initialize(renderer_, art_library->Search("Bullet"), world, bulletpos, direction);
    //     bullets.push_front(bullet);
    // }
}

void Player::Initialize() {}

void Player::set_input_device(InputDevice* input_device) {
    input_device_ = input_device;
}
