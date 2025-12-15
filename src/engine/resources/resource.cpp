#include "resource.h"
#include "loguru.hpp"
#include "fmt/format.h"


namespace resources
{
    std::map<int, std::string> REGISTRY = {};
    std::map<std::string, Resource*> LOCATION_TO_RESOURCE = {};
    std::map<std::string, int> LOCATION_TO_ID = {};


    Resource::Resource(unsigned int id, Type type, ResourceLocation* location) : id(id), type(type), location(location)
    {
    }


    unsigned int add(std::string uniqueResourceName)
    {
        unsigned int size = REGISTRY.size();

        for (auto const& [key, value] : REGISTRY)
        {
            if (value == uniqueResourceName)
            {
                LOG_F(WARNING, fmt::format("RESOURCE::Name for {} already exists", uniqueResourceName).c_str());
                return key;
            }
        }

        LOG_F(INFO, fmt::format("RESOURCE::REGISTERED: {}: {}", size, uniqueResourceName).c_str());
        REGISTRY.insert({size, uniqueResourceName});
        return size;
    }

    Resource *registerResource(Resource *resource)
    {
        std::string resourceName = REGISTRY[resource->id];

        LOCATION_TO_ID.insert({resourceName, resource->id});
        LOCATION_TO_RESOURCE.insert({resourceName, resource});

        return resource;
    }
}