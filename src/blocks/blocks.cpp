#include "blocks/blocks.h"

namespace blocks
{
    Block* AIR_BLOCK = nullptr;
    Block* DIRT_BLOCK = nullptr;
    Block* GRASS_BLOCK = nullptr;
    Block* COBBLESTONE_BLOCK = nullptr;
    Block* SNOW_BLOCK = nullptr;
    Block* WATER_BLOCK = nullptr;


    void initializeBlocks()
    {
        AIR_BLOCK = registerBlock(
            new Block(
                add("air_block"),
                BlockTextures()
            )
        );

        DIRT_BLOCK = registerBlock(
            new Block(
                add("dirt_block"),
                BlockTextures(resources::addTexture("./assets/images/dirt.png"))
            )
        );

        GRASS_BLOCK = registerBlock(
            new Block(
                add("grass_block"),
                BlockTextures(resources::addTexture("./assets/images/grass_block_side.png"))
                    .addUp(resources::addTexture("./assets/images/moss_block.png"))
                    .addDown(resources::addTexture("./assets/images/dirt.png"))
            )
        );

        COBBLESTONE_BLOCK = registerBlock(
            new Block(
                add("cobblestone"),
                BlockTextures(resources::addTexture("./assets/images/cobblestone.png"))
            )
        );

        SNOW_BLOCK = registerBlock(
            new Block(
                add("snow_block"),
                BlockTextures(resources::addTexture("./assets/images/snow.png"))
            )
        );

        WATER_BLOCK = registerBlock(
            new Block(
                add("water_block"),
                BlockTextures(resources::addTexture("./assets/images/water.png"))
            )
        );
    }

}