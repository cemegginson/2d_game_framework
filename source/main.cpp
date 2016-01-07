// Copyright 2015 Casey Megginson and Blaise Koch
#include <ctime>
#include <iostream>
#include <string>

// Media Libraries
#include "SDL.h"

// Physics
#include "Box2D/Box2D.h"

// Project Libraries
#include "util/definitions.h"
#include "core/app_2d.h"
#include "core/input_device.h"

#include "render/renderer.h"

int main(int argc, char* argv[]) {
    UNUSED(argc); UNUSED(argv);
    SDL_Init(0);

    Dispatcher::GetInstance();

    //========================================
    // Initialize the random number generator
    //========================================
    srand((unsigned int)time(NULL));

    //========================================
    // Base Game Constants
    //========================================
    uint32 screen_width = 800;
    uint32 screen_height = 600;

    //========================================
    // Construct Graphical Device
    //========================================
    Renderer* renderer = nullptr;
    renderer = new Renderer(screen_width, screen_height);

    if (!renderer->Initialize()) {
        printf("Graphics Device could not initialize!");
        exit(1);
    }

    //========================================
    // Construct Input Device
    //========================================
    InputDevice* input_device = new InputDevice();
    if (!input_device->Initialize()) {
        printf("Input Device could not initialize!");
        exit(1);
    }

    //========================================
    // Construct Game
    //========================================
    App2D* app = new App2D();
    if (!app->Initialize(renderer, input_device)) {
        printf("Game could not Initialize!");
        exit(1);  // this case will leak a lot of memory...
                  // should properly do destructor calls and proper shutdown
    }

    //========================================
    // Load Level
    //========================================
    app->Reset();
    std::string level_config_file = "Assets/Config/level.xml";
    if (!app->LoadLevel(level_config_file)) {
        printf("Game could not load level %s: ",
               level_config_file.c_str());
        exit(1);  // this case will leak a lot of memory...
                  // should properly do destructor calls and proper shutdown
    }

    // Start the app
    SDL_InitSubSystem(SDL_INIT_EVENTS);
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // Update the Input Device with the Event
            input_device->Update(&event);
        }
        app->Run();
    }

    //========================================
    // Clean-up
    //========================================
    if (app != nullptr) {
        delete app;
        app = nullptr;
    }

    if (input_device != nullptr) {
        delete input_device;
        input_device = nullptr;
    }

    if (renderer != nullptr) {
        delete renderer;
        renderer = nullptr;
    }

    SDL_Quit();

    return 0;
}
