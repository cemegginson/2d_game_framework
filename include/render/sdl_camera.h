#pragma once

#include "core/input_device.h"
#include "util/definitions.h"

class SDLCamera {
protected:
    InputDevice* input_device_;
    Vector2 position_;
    uint32 velocity_;

public:
    SDLCamera();
    ~SDLCamera();
    bool Initialize(InputDevice*);
    void Update(float32);
    Vector2 position();
};
