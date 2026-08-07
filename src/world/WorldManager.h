#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include "world/world.h"

class WorldManager{
    public:
        static WorldManager& instace() {
            static WorldManager worldManager;
            return worldManager;
        }
        WorldManager(const WorldManager&) = delete;
        WorldManager& operator=(const WorldManager&) = delete;

        template<typename T, typename ... Args>
        requires std::is_base_of_v<World, T>
        T* createWorld(Args&&... args) {
            auto world = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = world.get();
            m_worlds.push_back(std::move(world));

            m_activeWorld = ptr;
            return ptr;
        }

        template<typename T>
        requires std::is_base_of_v<World, T>
        T* getWorld() {
            for (auto& world : m_worlds) {
                if (auto casted = dynamic_cast<T*>(world.get()))
                    return casted;
            }

            return nullptr;
        }

        void setActiveWorld(World* world);
        void gracefulShutdown();
        World* getActiveWorld() const { return m_activeWorld; }
        

    
    private:

        WorldManager() = default;
        ~WorldManager() = default;

        std::vector<std::unique_ptr<World>> m_worlds;
        World* m_activeWorld = nullptr;


};