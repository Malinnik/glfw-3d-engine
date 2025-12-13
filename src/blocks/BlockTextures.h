#pragma once

#include "engine/resources/ResourceLocation.h"

namespace blocks
{

    class BlockTextures
    {
        public:
        resources::ResourceLocation* front;
        resources::ResourceLocation* back;
        resources::ResourceLocation* up;
        resources::ResourceLocation* down;
        resources::ResourceLocation* left;
        resources::ResourceLocation* right;
        
        BlockTextures();
        BlockTextures(resources::ResourceLocation* resourceLocation);
        
        BlockTextures addFront(resources::ResourceLocation* resource);
        BlockTextures addBack(resources::ResourceLocation* resource);
        BlockTextures addUp(resources::ResourceLocation* resource);
        BlockTextures addDown(resources::ResourceLocation* resource);
        BlockTextures addLeft(resources::ResourceLocation* resource);
        BlockTextures addRight(resources::ResourceLocation* resource);
        
        
        ~BlockTextures();
        
    };
    
}