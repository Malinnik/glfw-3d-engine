#include "block.h"
#include "loguru.hpp"
#include "fmt/format.h"


namespace blocks
{   
    std::map<unsigned int, std::string> REGISTRY = {};
    std::map<std::string, unsigned int> NAME_TO_ID = {};
    std::map<std::string, Block*> NAME_TO_BLOCK = {};

    // Block::Block() {}

    Block::Block(unsigned int id, BlockTextures textures) : id(id), textures(textures) {}

    unsigned int add(std::string uniqueBlockName)
    {
        unsigned int size = blocks::REGISTRY.size();
        
        for (auto const& [key, value] : blocks::REGISTRY)
        {
            if (value == uniqueBlockName)
            {
                LOG_F(WARNING, fmt::format("Name for {} already exists", uniqueBlockName).c_str());
                return key;
            }
        }
        
        blocks::REGISTRY.insert({size+1, uniqueBlockName});
        return size+1;
    }

    Block* get(unsigned int id)
    {
        std::string blockName = REGISTRY[id];
        return NAME_TO_BLOCK[blockName];
    }

    Block* registerBlock(Block* block)
    {
        std::string blockName = REGISTRY[block->id];
        
        NAME_TO_ID.insert({blockName, block->id});
        NAME_TO_BLOCK.insert({blockName, block});
        
        return block;
    }

    
}