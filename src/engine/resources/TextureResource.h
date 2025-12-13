#pragma once

#include "engine/resources/resource.h"
#include "engine/resources/ResourceLocation.h"

namespace resources
{
    class TextureResource : public Resource
    {
        public:
            
            Type type = Type::texture;
            TextureResource(unsigned int id, ResourceLocation* location);
        
    };
    
    ResourceLocation* addTexture(std::string pathToTexture);
}