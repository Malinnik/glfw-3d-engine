#pragma once

#include <cstdint>


namespace blocks
{
    class Block
    {
        uint8_t id;
            
    };

    class BlockRegistries
    {
        static uint8_t registerBlock(Block block);

    };

    

}

struct block {
    uint8_t id;
};