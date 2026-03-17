#pragma once
#include "blocks/block.h"

class WorldGenerator {
    public:
        static float getTerrainHeight(int x, int z);
        static float getBiomeAdjustedHeight(int x, int y, int z);
        static int getBlockType(int x, int y, int z);
        static void generate(unsigned int* blockIds, int cx, int cy, int cz);
        static void generate_v2(unsigned int *blockIds, int cx, int cy, int cz);
        static int randomSeed();
};