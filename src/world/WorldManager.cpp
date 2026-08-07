#include "WorldManager.h"

void WorldManager::setActiveWorld(World *world)
{
    if (world == nullptr)
        return;
    m_activeWorld = world;
}

void WorldManager::gracefulShutdown()
{
    for (auto& world : m_worlds) {
        world->save();
    }
}
