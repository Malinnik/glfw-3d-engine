#pragma once

class WorldGeneration {
    public:
        static inline float getTerrainHeight(int x, int y, int z);
        static float getBiomeAdjustedHeight(int x, int y, int z);
        static int getBlockType(int x, int y, int z);
        static int randomSeed();
};