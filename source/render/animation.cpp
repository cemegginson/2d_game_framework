// Copyright 2015 Casey Megginson and Blaise Koch
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "render/animation.h"

Animation::Animation(Actor2D* owner) {
    owner_ = owner;
    animations_ = std::map<std::string, std::vector<SDL_Rect>*>();
    current_animation_ = nullptr;
    current_frame_ = 0;
    frame_time_ = 0;

    Subscriber* s = new Subscriber(this);
    s->method = std::bind(&Animation::Update, this, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(s, "EVENT_COMPONENT_UPDATE");
    subscribers_.push_back(s);
}
Animation::~Animation() {
    // delete animations
    for (auto iter = animations_.begin(); iter != animations_.end(); ++iter) {
        delete iter->second;
    }

    while (subscribers_.size() > 0) {
        delete subscribers_.back();
        subscribers_.pop_back();
    }
}

void Animation::Update(std::shared_ptr<void> delta_time) {
    frame_time_ += *reinterpret_cast<float32*>(delta_time.get());

    if (frame_time_ >= .05) {
        frame_time_ = 0;
        current_frame_++;
        if (current_animation_ != nullptr &&
            current_frame_ >= current_animation_->size()) {
            current_frame_ = 0;
        }
        owner_->set_sprite_clip(current_animation_->at(current_frame_));
    }

    CopyEventState();
    UpdateState();

    if (!EventStateIsCurrent()) {
        ChooseAnimation();
    }
}

void Animation::Initialize() {
    state_.insert(std::pair<State, bool>(kFacingLeft, false));
    state_.insert(std::pair<State, bool>(kFacingRight, false));
    state_.insert(std::pair<State, bool>(kStationary, false));
    state_.insert(std::pair<State, bool>(kJumping, false));

    last_state_.insert(std::pair<State, bool>(kFacingLeft, false));
    last_state_.insert(std::pair<State, bool>(kFacingRight, false));
    last_state_.insert(std::pair<State, bool>(kStationary, false));
    last_state_.insert(std::pair<State, bool>(kJumping, false));

    SetAnimation("face-screen");
}

void Animation::AddAnimation(std::string name, std::vector<SDL_Rect>* animation) {
    // cleanup if animation was already defined (valgrind)
    if (animations_[name] != nullptr) delete animations_[name];

    animations_[name] = animation;
}

void Animation::UpdateState() {
    if (owner_->CheckEvent(kMoveLeft) && !owner_->CheckEvent(kMoveRight)) {
        state_.at(kStationary) = false;
        state_.at(kFacingLeft) = true;
        state_.at(kFacingRight) = false;
    }
    if (!owner_->CheckEvent(kMoveLeft) && owner_->CheckEvent(kMoveRight)) {
        state_.at(kStationary) = false;
        state_.at(kFacingRight) = true;
        state_.at(kFacingLeft) = false;
    }
    if (!owner_->CheckEvent(kMoveLeft) && !owner_->CheckEvent(kMoveRight)) {
        state_.at(kStationary) = true;
    }
    // if(owner_->CheckEvent(kMoveDown)) {
    //     ;
    // }
}

void Animation::ChooseAnimation() {
    if (state_.at(kStationary)) {
        if (state_.at(kFacingLeft)) {
            SetAnimation("standing-left");
        } else if (state_.at(kFacingRight)) {
            SetAnimation("standing-right");
        }
    } else if (!state_.at(kStationary)) {
        if (state_.at(kFacingLeft)) {
            SetAnimation("run-left");
        } else if (state_.at(kFacingRight)) {
            SetAnimation("run-right");
        }
    }
}

void Animation::SetAnimation(std::string name) {
    frame_time_ = 0;
    current_animation_ = animations_.at(name);
}

bool Animation::EventStateIsCurrent() {
    State temp;
    for (auto iter = state_.begin(); iter != state_.end(); ++iter) {
        temp = iter->first;
        if (state_.at(temp) != last_state_.at(temp)) {
            return false;
        }
    }

    return true;
}

void Animation::CopyEventState() {
    State temp;
    for (auto iter = state_.begin(); iter != state_.end(); ++iter) {
        temp = iter->first;
        last_state_.at(temp) = state_.at(temp);
    }
}
