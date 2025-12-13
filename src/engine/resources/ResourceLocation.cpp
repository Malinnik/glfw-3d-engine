#include "ResourceLocation.h"
#include "engine/resources/resource.h"

resources::ResourceLocation::ResourceLocation(std::string path) : path(path)
{
}

resources::ResourceLocation::~ResourceLocation()
{
}

unsigned int resources::ResourceLocation::getID()
{
    return resources::LOCATION_TO_ID[path];
}