#pragma once

#include "engine/transform.h"
#include "engine/camera/camera.h"

namespace entity {
    class IControllable{
        virtual void handleInputEvent() const = 0;
    };

    class Entity{
        public:
            Transform transform;

    };


    namespace player {
        class Player : public Entity, public IControllable {
            public:
                Player();
                Player(Transform position);

            private:
                void handleInputEvent();
                Camera* camera;

        };
            
    }
}