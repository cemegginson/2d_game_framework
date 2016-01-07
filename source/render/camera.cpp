#include "render/camera.h"

Camera::Camera() {}

Camera::~Camera() {}

bool Camera::Initialize(InputDevice* input_device) {
    input_device_ = input_device;
    velocity_ = 400;
    position_.x = 0;
    position_.y = 0;
    return true;
}

void Camera::Update(float32 delta_time) {
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

Vector2 Camera::position() {
    return position_;
}
