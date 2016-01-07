// #pragma once
//
// #include <list>
// #include <string>
//
// #include "Box2D/Box2D.h"
//
// #include "art_library/ComponentLibrary.h"
// #include "art_library/ArtAssetLibrary.h"
//
// #include "core/timer.h"
// #include "render/sdl_camera.h"
// #include "render/actor_2d.h"
//
// #include "render/renderer.h"
//
// #include "virtual/component.h"
// #include "virtual/app.h"
//
// class App2D : public App {
// protected:
//     ComponentLibrary* component_factories_;
//     ArtAssetLibrary* art_library_;
//     Renderer* renderer_;
//     InputDevice* input_device_;
//     Timer* timer_;
//     SDLCamera* camera_;
//
//     std::list<Actor2D*> actors_;
//
//     b2World* world_;
//     // float32 physics_delta_time_;
//     int32 velocity_iterations_;
//     int32 position_iterations_;
//     // int32 particle_iterations_;
//
// public:
//     App2D();
//     ~App2D();
//     bool Initialize(Renderer*, InputDevice*);
//     void Reset();
//     bool LoadLevel(std::string);
//     void Run();
//     void Update(float32);
//
//     // cmake adds NDEBUG by default for debug builds
//     #ifndef NDEBUG
//     Subscriber* debug_subscriber_;
//     void printFrameRate(std::shared_ptr<void> delta_time);
//     #endif
//
// };
