#include "render/sdl_camera.h"

SDLCamera::SDLCamera() {}

SDLCamera::~SDLCamera() {}

bool SDLCamera::Initialize(InputDevice* input_device) {
    input_device_ = input_device;
    velocity_ = 400;
    position_.x = 0;
    position_.y = 0;
    return true;
}

void SDLCamera::Update(float32 delta_time) {
    if (input_device_->IsPressed(kGameUp)) {
        position_.y += velocity_ * delta_time;
    }
    if (input_device_->IsPressed(kGameDown)) {
        position_.y -= velocity_ * delta_time;
    }
    if (input_device_->IsPressed(kGameLeft)) {
        position_.x += velocity_ * delta_time;
    }
    if (input_device_->IsPressed(kGameRight)) {
        position_.x -= velocity_ * delta_time;
    }
    return;
}

Vector2 SDLCamera::position() {
    return position_;
}
