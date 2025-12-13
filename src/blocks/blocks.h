#include "blocks/block.h"
#include "engine/resources/TextureResource.h"

namespace blocks
{
    static Block* AIR_BLOCK = registerBlock(
        new Block(
            add("air_block"),
            BlockTextures()
        )
    );

    static Block* DIRT_BLOCK = registerBlock(
        new Block(
            add("dirt_block"),
            BlockTextures(resources::addTexture("./assets/images/dirt.png"))
        )
    );

    static Block* GRASS_BLOCK = registerBlock(
        new Block(
            add("grass_block"),
            BlockTextures(resources::addTexture("./assets/images/grass_block_side.png"))
                .addUp(resources::addTexture("./assets/images/moss_block.png"))
                .addDown(resources::addTexture("./assets/images/dirt.png"))
        )
    );

    static Block* COBBLESTONE_BLOCK = registerBlock(
        new Block(
            add("cobblestone"),
            BlockTextures(resources::addTexture("./assets/images/cobblestone.png"))
        )
    );

    static Block* SNOW_BLOCK = registerBlock(
        new Block(
            add("snow_block"),
            BlockTextures(resources::addTexture("./assets/images/snow.png"))
        )
    );

    static Block* WATER_BLOCK = registerBlock(
        new Block(
            add("water_block"),
            BlockTextures(resources::addTexture("./assets/images/water.png"))
        )
    );

}