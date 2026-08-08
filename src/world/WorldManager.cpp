#include "WorldManager.h"

void WorldManager::setActiveWorld(World *world)
{
    if (world == nullptr)
        return;
    m_activeWorld = world;
}

void WorldManager::clearWorlds()
{
    m_worlds.clear();
    m_activeWorld = nullptr;
}

void WorldManager::gracefulShutdown()
{
    for (auto& world : m_worlds) {
        world->save();
    }
    clearWorlds();
}
