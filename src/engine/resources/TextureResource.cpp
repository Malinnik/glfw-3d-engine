#include "TextureResource.h"


namespace resources
{

    TextureResource::TextureResource(unsigned int id, ResourceLocation* location) : Resource(id, type, location)
    {
    }
    
    ResourceLocation* addTexture(std::string pathToTexture)
    {
        Resource* resource = registerResource(new TextureResource(
            resources::add(pathToTexture),
            new ResourceLocation(pathToTexture)
        ));
        
        return resource->location;
    }
    
}