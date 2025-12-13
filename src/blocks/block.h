#pragma once

#include <cstdint>
#include <map>

#include "engine/resources/ResourceLocation.h"
#include "blocks/BlockTextures.h"

namespace blocks
{  
    class Block
    {
        public:
            unsigned int id;
            BlockTextures textures;
            // Block();
            Block(unsigned int id, BlockTextures textures);
        
    };

    extern std::map<unsigned int, std::string> REGISTRY;
    extern std::map<std::string, unsigned int> NAME_TO_ID;
    extern std::map<std::string, Block*> NAME_TO_BLOCK;
    
    unsigned int add(std::string uniqueBlockName);
    Block* get(unsigned int id);
    Block* registerBlock(Block* block);
}
