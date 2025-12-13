#pragma once
#include <map>
#include <string>

namespace resources
{
    class ResourceLocation
    {
        public:
            std::string path;
            ResourceLocation(std::string path);
            ~ResourceLocation();
            unsigned int getID();
        
    };    
}