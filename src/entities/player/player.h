#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <vector>


#include "config.h"
#include "window/events.h"
#include "engine/transform.h"
#include "engine/camera/camera.h"
#include "engine/crosshair.h"
#include "engine/BlockSelector.h"
#include "engine/AABB.h"
#include "world/chunks.h"
#include "blocks/blocks.h"

namespace entity {
    
    class Entity{
            public:
                Transform transform;

                Entity() = default;
                virtual ~Entity() = default;

                virtual void onUpdate(float delta) {}
                virtual void onRender() {}
        };

    class IControlProvider {
        public:
            virtual ~IControlProvider() = default;
            virtual void processMovement(float delta) = 0;
            virtual void processMouseMotion() = 0;
            virtual void processBlockInteraction() = 0;
    };

    class IControllable {
        public:
            void handleControl(float delta) {
                if (m_controller)
                {
                    m_controller->processMovement(delta);
                    m_controller->processMouseMotion();
                    m_controller->processBlockInteraction();
                }
            }
            void setController(std::unique_ptr<IControlProvider> controller) { 
                m_controller = std::move(controller); 
            }
     
        protected:
            std::unique_ptr<IControlProvider> m_controller;
    };
    
    class PhysicsComponent {
        public:
            PhysicsComponent();

            bool gravityEnabled = true;

            void update(float delta, Entity& owner);

            void setSize(float width, float height);
            AABB getAABB(const glm::vec3& position) const;

            glm::vec3 velocity = glm::vec3(0.0f);
            bool onGround = false;

            float gravity = 9.8f;
            float jumpStrength = 8.7f;

            using BlockQueryFunc = std::function<std::vector<glm::ivec3>(const AABB&)>;
            void setBlockQuery(BlockQueryFunc func) { m_blockQuery = func; }

        private:
            void moveAxis(float amount, int axis, Entity& owner);
            float m_width = 0.6f;
            float m_height = 1.8f;
            BlockQueryFunc m_blockQuery;

        };
        
    class SurvivalController : public IControlProvider {
        public:
            explicit SurvivalController(class PhysicsComponent* physicsComponent, class Camera* camera, glm::ivec3* targetBlock, bool* hasTarget);
            void processMovement(float delta) override;
            void processMouseMotion() override;
            void processBlockInteraction() override;
        private:
            PhysicsComponent* physics;
            Camera* camera;
            glm::ivec3* targetBlock;
            bool* hasTarget;
    };

    class CreativeController : public IControlProvider {
        public:
            explicit CreativeController(class PhysicsComponent* physicsComponent, class Camera* camera, glm::ivec3* targetBlock, bool* hasTarget, float* speed);
            void processMovement(float delta) override;
            void processMouseMotion() override;
            void processBlockInteraction() override;
        private:
            PhysicsComponent* physics;
            Camera* camera;
            glm::ivec3* targetBlock;
            bool* hasTarget;
            float* speed;
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

            private:
                PhysicsComponent physics;

                Camera* camera = nullptr;
                Crosshair* crosshair = nullptr;
                BlockSelector* blockSelector = nullptr;

                glm::ivec3 targetBlock;
                bool hasTarget = false;

                float eyeHeight = 1.62f;
                float speed = 2.0f;
        };
            
    }
}