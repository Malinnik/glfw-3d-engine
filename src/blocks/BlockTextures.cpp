#include "BlockTextures.h"
#include "engine/resources/TextureResource.h"

namespace blocks
{
    BlockTextures::BlockTextures()
    {
        resources::ResourceLocation* error_resource = resources::addTexture("./assets/images/error.png");

        front = error_resource;
        back = error_resource;
        up = error_resource;
        down = error_resource;
        left = error_resource;
        right = error_resource;

    }

    BlockTextures::BlockTextures(resources::ResourceLocation *resourceLocation) : front(resourceLocation), back(resourceLocation), up(resourceLocation), down(resourceLocation), left(resourceLocation), right(resourceLocation)
    {}

    BlockTextures BlockTextures::addFront(resources::ResourceLocation *resource)
    {
        front = resource;
        return *this;
    }

    BlockTextures BlockTextures::addBack(resources::ResourceLocation *resource)
    {
        back = resource;
        return *this;
    }

    BlockTextures BlockTextures::addUp(resources::ResourceLocation *resource)
    {
        up = resource;
        return *this;
    }

    BlockTextures BlockTextures::addDown(resources::ResourceLocation *resource)
    {
        down = resource;
        return *this;
    }

    BlockTextures BlockTextures::addLeft(resources::ResourceLocation *resource)
    {
        left = resource;
        return *this;
    }

    BlockTextures BlockTextures::addRight(resources::ResourceLocation *resource)
    {
        right = resource;
        return *this;
    }

    // BlockTextures::~BlockTextures()
    
    // {
    //     delete front;
    //     delete back;
    //     delete up;
    //     delete down;
    //     delete left;
    //     delete right;
    // }

}