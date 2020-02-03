#include "pch.h"
#include <gamelib_actor.hpp>
#include <gamelib_locator.hpp>

namespace GameLib {
    unsigned Actor::idSource_{ 0 };

    Actor::Actor(InputComponent* input, ActorComponent* actor, PhysicsComponent* physics, GraphicsComponent* graphics)
        : transform(1.0f)
        , addlTransform(1.0f)
        , input_(input)
        , actor_(actor)
        , physics_(physics)
        , graphics_(graphics) {
        id_ = idSource_++;
    }

    Actor::~Actor() {
        /*delete input_;
        delete physics_;
        delete graphics_;
        delete actor_;*/
    }

    void Actor::beginPlay() {}

    void Actor::update(float deltaTime, World& world, Graphics& graphics) {
        dt = deltaTime;
        if (input_)
            input_->update(*this);
        if (actor_)
            actor_->update(*this, world);
        if (physics_)
            physics_->update(*this, world);
        if (graphics_)
            graphics_->update(*this, graphics);
    }

    void Actor::collidedWith(Actor * hit) {}

    void Actor::startOverlap(const_weak_ptr otherObject) {}

    void Actor::endOverlap(const_weak_ptr otherObject) {}

    PlayerActor::PlayerActor(InputComponent* input, ActorComponent* actor, PhysicsComponent* physics, GraphicsComponent* graphics)
        :Actor::Actor(input, actor, physics, graphics)
    {
        playerHP_=10;
    }

    PlayerActor::~PlayerActor() {
        Actor::~Actor();
    }

    void PlayerActor::collidedWith(Actor * hit)
    {
        HFLOGDEBUG("Actor %d colliding with Actor %d",id_,hit->id_);
        if(hit->id_>0)
            playerHP_--;
        hit->position.x=38;
        hit->position.y=20;
    }
}
