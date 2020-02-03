#include "pch.h"
#include <gamelib_random.hpp>
#include <gamelib_locator.hpp>
#include <gamelib_component.hpp>
#include <math.h>
#include <vector>

namespace GameLib {
    void SimpleInputComponent::update(Actor& actor) {
        auto axis1 = Locator::getInput()->axis1X;
        if (axis1)
            actor.velocity.x = axis1->getAmount();
        auto axis2 = Locator::getInput()->axis1Y;
        if (axis2)
            actor.velocity.y = axis2->getAmount();
    }

    void RandomInputComponent::update(Actor& actor) {
        actor.velocity.x = random.normal();
        actor.velocity.y = random.normal();
        glm::normalize(actor.velocity);
    }

    void SimpleActorComponent::update(Actor& actor, World& world) {
        // We could make decisions here, start actions, etc
    }

    void RandomActorComponent::update(Actor& actor, World& world) {
        for (auto& a : world.actors) {
            if (a->getId() == actor.getId())
                continue;
        }
    }

    void SimplePhysicsComponent::update(Actor& actor, World& world) {
        actor.position += actor.dt * actor.speed * actor.velocity;
        if (actor.clipToWorld) {
            actor.position.x = clamp<float>(actor.position.x, 0, (float)world.worldSizeX - actor.size.x);
            actor.position.y = clamp<float>(actor.position.y, 0, (float)world.worldSizeY - actor.size.y);
        }
    }

    void ColliderPhysicsComponent::update(Actor& actor, World& world) {
        auto oldPosition=actor.position;
        SimplePhysicsComponent::update(actor, world);
        //Hit wall?
        std::vector<Tile> tilesToTest;
        double frac;
        if(modf(actor.position.x,&frac)!=0&&modf(actor.position.y,&frac)!=0)
        {
            tilesToTest.push_back(world.getTile(floor(actor.position.x),floor(actor.position.y)));
            tilesToTest.push_back(world.getTile(floor(actor.position.x),ceil(actor.position.y)));
            tilesToTest.push_back(world.getTile(ceil(actor.position.x),floor(actor.position.y)));
            tilesToTest.push_back(world.getTile(ceil(actor.position.x),ceil(actor.position.y)));
        }
        else if(modf(actor.position.x,&frac)!=0)
        {
            tilesToTest.push_back(world.getTile(floor(actor.position.x),actor.position.y));
            tilesToTest.push_back(world.getTile(ceil(actor.position.x),actor.position.y));
        }
        else if(modf(actor.position.y,&frac)!=0)
        {
            tilesToTest.push_back(world.getTile(actor.position.x,floor(actor.position.y)));
            tilesToTest.push_back(world.getTile(actor.position.x,ceil(actor.position.y)));
        }
        else
        {
            tilesToTest.push_back(world.getTile(actor.position.x,actor.position.y));
        }
        for(Tile& t:tilesToTest)
        {
            if(851<=t.charDesc&&t.charDesc<=861)
                actor.position=oldPosition;
            if(901==t.charDesc)
                world.win=true;
        }
        
        //Hit other actor? only good for same size actors
        for(Actor * a:world.actors)
        {
            if(actor.id_!=a->id_)
            {
                if((a->position.x<=actor.position.x&&actor.position.x<=a->position.x+1 && a->position.y<=actor.position.y&&actor.position.y<=a->position.y+1) ||
                    (a->position.x<=actor.position.x+1&&actor.position.x+1<=a->position.x+1 && a->position.y<=actor.position.y&&actor.position.y<=a->position.y+1) ||
                    (a->position.x<=actor.position.x&&actor.position.x<=a->position.x+1 && a->position.y<=actor.position.y+1&&actor.position.y+1<=a->position.y+1) ||
                    (a->position.x<=actor.position.x+1&&actor.position.x+1<=a->position.x+1 && a->position.y<=actor.position.y+1&&actor.position.y+1<=a->position.y+1))
                {
                    actor.collidedWith(a);
                }
            }
        }
    }

    void SimpleGraphicsComponent::update(Actor& actor, Graphics& graphics) {
        glm::vec3 tileSize{ graphics.getTileSizeX(), graphics.getTileSizeY(), 0 };
        glm::vec3 pos = actor.position * tileSize;
        int flipFlags = actor.spriteFlipX ? 1 : actor.spriteFlipY ? 2 : 0;
        graphics.draw(actor.spriteLibId, actor.spriteId, pos.x, pos.y, flipFlags);
    }
}
