#pragma once
#include <map>

#include "engine/resources/ResourceLocation.h"

// namespace resources
// {
//     class ResourceLocation;
// }

namespace resources
{
    enum class Type {texture};
    
    class Resource
    {
        public:
            unsigned int id;
            Type type;
            ResourceLocation* location;
        
        Resource(unsigned int id, Type type, ResourceLocation* location);
    };


    extern std::map<int, std::string> REGISTRY;
    extern std::map<std::string, Resource*> LOCATION_TO_RESOURCE;
    extern std::map<std::string, int> LOCATION_TO_ID;

    unsigned int add(std::string uniqueResourceName);
    Resource* registerResource(Resource* resource);
}