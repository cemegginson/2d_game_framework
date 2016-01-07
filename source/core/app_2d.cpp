#include <iostream>
#include <string>

// #include "ContactListener.h"
#include "core/app_2d.h"
#include "util/box2d_utils.h"
#include "core/PugiXML/pugixml.hpp"

App2D::App2D() {
    component_factories_ = nullptr;
    art_library_ = nullptr;
    renderer_ = nullptr;
    input_device_ = nullptr;
    timer_ = nullptr;
    camera_ = nullptr;
    // physics_delta_time_ = 1.0/100.0;
    velocity_iterations_ = 8;
    position_iterations_ = 3;
    // particle_iterations_ = 3;
}

App2D::~App2D() {
    // #ifdef NDEBUG
    //     if (debug_subscriber_ != nullptr) {
    //         delete debug_subscriber_;
    //         debug_subscriber_ = nullptr;
    //     }
    // #endif

    while (actors_.size() > 0) {
        delete actors_.back();
        actors_.pop_back();
    }
    if (component_factories_ != nullptr) {
        delete component_factories_;
        component_factories_ = nullptr;
    }
    if (art_library_ != nullptr) {
        delete art_library_;
        art_library_ = nullptr;
    }
    if (camera_ != nullptr) {
        delete camera_;
        camera_ = nullptr;
    }
    if (timer_ != nullptr) {
        delete timer_;
        timer_ = nullptr;
    }
    if (world_ != nullptr) {
        delete world_;
        world_ = nullptr;
    }
}

bool App2D::Initialize(Renderer* renderer,
                      InputDevice* input_device) {
    renderer_ = renderer;
    input_device_ = input_device;

    camera_ = new Camera();
    camera_->Initialize(input_device_);

    // Load sprites
    art_library_ = new ArtAssetLibrary();
    art_library_->LoadAssets(renderer_, camera_);

    timer_ = new Timer();
    timer_->Start();

    uint32 screen_width = 800;
    uint32 screen_height = 600;

    // Initialize Physics world
    const b2Vec2 gravity(0, 150);
    world_ = new b2World(gravity);

    #ifndef NDEBUG
        debug_subscriber_ = new Subscriber(this);
        debug_subscriber_->method = std::bind(&App2D::printFrameRate, this, std::placeholders::_1);
        Dispatcher::GetInstance()->AddEventSubscriber(debug_subscriber_, "EVENT_COMPONENT_UPDATE");
    #endif

    // Set boundaries of world (Render->Physics)
    const b2Vec2 vTopLeft = b2Vec2(RW2PW(0), RW2PW(0));
    const b2Vec2 vTopRight = b2Vec2(RW2PW((int32)screen_width), RW2PW(0));
    const b2Vec2 vBottomLeft = b2Vec2(RW2PW(0), RW2PW((int32)screen_height - 60));
    const b2Vec2 vBottomRight = b2Vec2(RW2PW((int32)screen_width), RW2PW((int32)screen_height - 60));

    //
    // Create the world boundaries
    //
    b2BodyDef bd;
    b2Body* edge = world_->CreateBody(&bd);
    b2EdgeShape shape;

    // Create top boundary
    shape.Set(vTopLeft, vTopRight);
    edge->CreateFixture(&shape, 0);

    // Create bottom boundary
    shape.Set(vBottomLeft, vBottomRight);
    edge->CreateFixture(&shape, 0);

    // Create left boundary
    shape.Set(vBottomLeft, vTopLeft);
    edge->CreateFixture(&shape, 0);

    // Create right boundary
    shape.Set(vBottomRight, vTopRight);
    edge->CreateFixture(&shape, 0);

    // Create Factories
    component_factories_ = new ComponentLibrary();

    component_factories_->AddFactory("Animation", reinterpret_cast<ComponentFactory*>(new AnimationFactory()));
    // component_factories_->AddFactory("Carrier", reinterpret_cast<ComponentFactory*>(new CarrierFactory()));
    // component_factories_->AddFactory("Infantry", reinterpret_cast<ComponentFactory*>(new InfantryFactory()));
    component_factories_->AddFactory("Player", reinterpret_cast<ComponentFactory*>(new PlayerFactory(input_device_)));
    // component_factories_->AddFactory("RigidCircle", reinterpret_cast<ComponentFactory*>new RigidCircleFactory(world_));
    component_factories_->AddFactory("RigidRectangle", reinterpret_cast<ComponentFactory*>(new RigidRectangleFactory(world_)));
    component_factories_->AddFactory("Sprite", reinterpret_cast<ComponentFactory*>(new SpriteFactory(renderer_, art_library_)));

    // ContactListener* contact_listener = new ContactListener();
    // world_->SetContactListener(contact_listener);

    return true;
}

void App2D::Reset() {
    // if(!actors_.empty()){
    //     for (auto iter = actors_.begin(); iter != actors_.end(); iter++) {
    //         delete *iter;
    //     }
    // }
}

bool App2D::LoadLevel(std::string file) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file.c_str());
    if (result) {
        pugi::xml_node Level = doc.child("Level");

        // Loop through Actor XML nodes
        for (pugi::xml_node actor_node : Level.children("Actor")) {
            std::string name = actor_node.attribute("name").value();
            bool controllable = std::stoi(actor_node.attribute("controllable").value());

            Vector2 position;
            position.x = std::stof(actor_node.attribute("x").value());
            position.y = std::stof(actor_node.attribute("y").value());
            float32 angle = std::stof(actor_node.attribute("angle").value());

            Actor2D* new_actor = new Actor2D();
            new_actor->Initialize(name, position, angle, controllable);

            // Loop through Component XML nodes
            for (pugi::xml_node component_node : actor_node.children("Component")) {
                std::string type = component_node.attribute("type").value();
                Component* new_component = reinterpret_cast<Component*>(component_factories_->Search(type)->Create(new_actor, component_node));
                new_actor->AddComponent(new_component);
            }
            actors_.push_back(new_actor);
        }
    }
    return true;
}

void App2D::Run() {
    Update(timer_->DeltaTime());
    timer_->Update();

    renderer_->PreDraw();
    renderer_->Draw();
    renderer_->PostDraw();
}

void App2D::Update(float32 delta_time) {
    // Update Camera position
    camera_->Update(delta_time);

    Dispatcher::GetInstance()->DispatchEvent("EVENT_COMPONENT_UPDATE",    std::make_shared<float32>(delta_time));
    Dispatcher::GetInstance()->DispatchEvent("EVENT_PHYSICS_UPDATE",      std::make_shared<float32>(delta_time));

    Dispatcher::GetInstance()->Pump();
	Dispatcher::GetInstance()->NonSerialProcess(); //do processing of subscriber objects that can't be run in the thread pool

    // keep this condition low enough to keep the game feeling non-laggy but high enough to keep threads busy
    while (Dispatcher::GetInstance()->QueueSize() > 128) {
        std::cout << "Thread queue is too full...  waiting a bit" << std::endl;
        sleep(1);
    }

    // Cycle through every objects' Update method
    // for (auto iter = actors_.begin(); iter != actors_.end(); ++iter) {
    //    (*iter)->Update(delta_time);
    // }
    world_->Step(delta_time, velocity_iterations_, position_iterations_);
    // world_->Step(physics_delta_time_, velocity_iterations_, position_iterations_, particle_iterations_);
}

#ifndef NDEBUG
    void App2D::printFrameRate(std::shared_ptr<void> delta_time) {
        float32 time = *reinterpret_cast<float32*>(delta_time.get());
        std::cout << "FPS:\t" << 1.0/time << std::endl;
    }
#endif
