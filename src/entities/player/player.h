#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "config.h"
#include "window/events.h"
#include "engine/transform.h"
#include "engine/camera/camera.h"
#include "engine/crosshair.h"
#include "engine/BlockSelector.h"
#include "world/chunks.h"
#include "blocks/blocks.h"

namespace entity {


    class IControllable{
        public:
            virtual ~IControllable() = default;
            virtual void processMovement(float delta) = 0;
            virtual void processMouseMotion() = 0;
        
    };

    class Entity{
        public:
            Transform transform;

            Entity() = default;
            virtual ~Entity() = default;

            virtual void onUpdate(float delta) {}
            virtual void onRender() {}

    };


    namespace player {

        class Player : public Entity, public IControllable {
            public:
                Player();
                Player(Transform position);
                ~Player() override;
                
                Camera* getCamera() const { return camera; }
                void onUpdate(float delta) override;
                void onRender() override;
                
                float speed = 10.0f;

            private:
                void processMovement(float delta) override;
                void processMouseMotion() override;
                void processBlockInteraction();


                Camera* camera = nullptr;
                Crosshair* crosshair = nullptr;
                BlockSelector* blockSelector = nullptr;

                glm::ivec3 targetBlock;
                bool hasTarget = false;
                

        };
            
    }
}