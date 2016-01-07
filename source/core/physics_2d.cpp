// Copyright 2015 Casey Megginson and Blaise Koch

#include <math.h>

#include "util/box2d_utils.h"
#include "core/physics_2d.h"

Rigidbody::Rigidbody()
    {}

Rigidbody::~Rigidbody() {
    world_->DestroyBody(body_);
}

/*
// RigidCircle methods

RigidCircle::RigidCircle(Actor2D* owner) :
                         Rigidbody(),
                         Component(owner) {}

RigidCircle::~RigidCircle() {
    while(subscribers.size() > 0) {
        delete subscribers.back();
        subscribers.pop_back();
    }
}

void RigidCircle::Initialize(b2World* world,
                             b2BodyDef body_definition,
                             b2FixtureDef shape_fixture_definition) {
    world_ = world;
    body_ = world->CreateBody(&body_definition);
    body_->CreateFixture(&shape_fixture_definition);
    controllable_ = owner_->IsControllable();
}

void RigidCircle::Update(std::shared_ptr<void> delta_time) {
    b2Vec2 new_direction;
    float32 angle = body_->GetAngle();
    float32 angular_velocity = 0;
    float32 velocity = 30;
    new_direction.x = 0;
    new_direction.y = 0;

    if (controllable_) {
        if (owner_->CheckEvent(kTurnLeft)) {
            angular_velocity -= PI;
        }
        if (owner_->CheckEvent(kTurnRight)) {
            angular_velocity += PI;
        }
        if (owner_->CheckEvent(kMoveUp)) {
            new_direction.x += velocity * cos(angle - PI/2);
            new_direction.y += velocity * sin(angle - PI/2);
        }
        if (owner_->CheckEvent(kMoveDown)) {
            new_direction.x -= velocity * cos(angle - PI/2);
            new_direction.y -= velocity * sin(angle - PI/2);
        }
        if (owner_->CheckEvent(kMoveLeft)) {
            new_direction.x -= 1;
        }
        if (owner_->CheckEvent(kMoveRight)) {
            new_direction.x += 1;
        }

        body_->SetLinearVelocity(new_direction);
        body_->SetAngularVelocity(angular_velocity);
    }
    ExportPosition();
    ExportAngle();
}

b2Vec2 RigidCircle::ExportPosition() {
    b2Vec2 physics_position = body_->GetPosition();
    Vector2 render_position;
    render_position.x = PW2RW(physics_position.x);
    render_position.y = PW2RW(physics_position.y);
    owner_->set_position(render_position);
    return physics_position;
}

float32 RigidCircle::ExportAngle() {
    float32 physics_angle = body_->GetAngle();
    owner_->set_angle(PW2RWAngle(physics_angle));
    return physics_angle;
}

b2Vec2 RigidCircle::ImportPosition() {
    Vector2 render_position = owner_->position();
    b2Vec2 physics_position;
    physics_position.x = RW2PW(render_position.x);
    physics_position.y = RW2PW(render_position.y);
    return physics_position;
}

float32 RigidCircle::ImportAngle() {
    return RW2PWAngle(owner_->angle());
}
*/


// RigidRectangle methods

RigidRectangle::RigidRectangle(Actor2D* owner) :
                               Rigidbody() {
    owner_ = owner;
}

RigidRectangle::~RigidRectangle() {
    while (subscribers.size() > 0) {
        delete subscribers.back();
        subscribers.pop_back();
    }
}

void RigidRectangle::Initialize(b2World* world,
                                b2BodyDef body_definition,
                                b2FixtureDef shape_fixture_definition) {
    world_ = world;
    body_ = world->CreateBody(&body_definition);
    body_->CreateFixture(&shape_fixture_definition);
    controllable_ = owner_->IsControllable();

    Subscriber* s = new Subscriber(this);
    s->method = std::bind(&RigidRectangle::Update, this, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(s, "EVENT_PHYSICS_UPDATE");
    subscribers.push_back(s);
}

void RigidRectangle::Update(std::shared_ptr<void> delta_time) {
    float32 time = *reinterpret_cast<float32*>(delta_time.get());
    b2Vec2 new_direction{0, 0};
    // float32 angle = body_->GetAngle();
    float32 angular_velocity = 0;

    b2Vec2 jump;
    jump.x = 0;
    jump.y = 300;

    if (controllable_) {
        if (owner_->CheckEvent(kTurnLeft)) {
            angular_velocity -= PI*time;
        }
        if (owner_->CheckEvent(kTurnRight)) {
            angular_velocity += PI*time;
        }
        if (owner_->CheckEvent(kMoveUp)) {

            // I'm not sure if these checks are correct but we need to do them
            // because some programmers don't understand default paramaters
            // #ifdef WIN32
                body_->ApplyForceToCenter(jump, 1);
            // #endif
            // #ifndef WIN32
                // body_->ApplyForceToCenter(jump);
            // #endif
        }
        if (owner_->CheckEvent(kMoveDown)) {}
        if (owner_->CheckEvent(kMoveLeft)) {
            new_direction.x -= 30;
        }
        if (owner_->CheckEvent(kMoveRight)) {
            new_direction.x += 30;
        }

        body_->SetLinearVelocity(new_direction);
        body_->SetAngularVelocity(angular_velocity);
    }
    ExportPosition();
    ExportAngle();
}

b2Vec2 RigidRectangle::ExportPosition() {
    b2Vec2 physics_position = body_->GetPosition();
    Vector2 render_position;
    render_position.x = PW2RW(physics_position.x);
    render_position.y = PW2RW(physics_position.y);
    owner_->set_position(render_position);
    return physics_position;
}

float32 RigidRectangle::ExportAngle() {
    float32 physics_angle = body_->GetAngle();
    owner_->set_angle(PW2RWAngle(physics_angle));
    return physics_angle;
}

b2Vec2 RigidRectangle::ImportPosition() {
    Vector2 render_position = owner_->position();
    b2Vec2 physics_position;
    physics_position.x = RW2PW(render_position.x);
    physics_position.y = RW2PW(render_position.y);
    return physics_position;
}

float32 RigidRectangle::ImportAngle() {
    return RW2PWAngle(owner_->angle());
}
