#include "generation.h"

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include <random>
#include <iostream>

#include "blocks/blocks.h"

float WorldGeneration::getTerrainHeight(int x, int y, int z)
{
    float height = 0.0f;

    float baseFrequency = 0.01f;
    float amplitude = 40.0f;
    float persistance = 0.5f;

    int octaves = 6;
    float freq = baseFrequency;
    float amp = amplitude;

    int seed = WorldGeneration::randomSeed();

    for (int i = 0; i < octaves; i++)
    {
        float noiseValue = glm::perlin(glm::vec2(x * freq, z * freq));
        float normalizedNoise = (noiseValue + 1.0f) * 0.5f;

        height += normalizedNoise * amp;

        freq *= 2.0f;
        amp *= persistance;
    }

    height += 32.0f;
    return height;
}

inline float WorldGeneration::getBiomeAdjustedHeight(int x, int y, int z) {
    // Шум для определения биома (низкая частота)
    glm::vec2 biomeCoord(x * 0.001f, z * 0.001f);
    float biomeNoise = glm::perlin(biomeCoord);
    
    // Шум для высоты (средняя частота)
    glm::vec2 heightCoord(x * 0.01f, z * 0.01f);
    float baseHeight = glm::perlin(heightCoord);
    
    // Шум для деталей (высокая частота)
    glm::vec2 detailCoord(x * 0.05f, z * 0.05f);
    float detailNoise = glm::perlin(detailCoord);
    
    // Комбинируем с весами
    float height = 0.0f;
    
    if (biomeNoise > 0.5f) {
        // Горы
        height = 80.0f + baseHeight * 50.0f + detailNoise * 10.0f;
    }
    else if (biomeNoise > 0.0f) {
        // Холмы
        height = 60.0f + baseHeight * 30.0f + detailNoise * 5.0f;
    }
    else {
        // Равнины
        height = 50.0f + baseHeight * 10.0f + detailNoise * 2.0f;
    }
    
    // Добавляем дополнительные особенности
    // Пещеры (используем 3D шум)
    if (y < height - 10) {
        glm::vec3 caveCoord(x * 0.05f, y * 0.1f, z * 0.05f);
        float caveNoise = glm::perlin(caveCoord);
        return caveNoise;
        // if (caveNoise > 0.7f) return AIR_BLOCK;  // Пещера
    }
    
    return height;
}

int WorldGeneration::getBlockType(int x, int y, int z)
{
    float height = WorldGeneration::getTerrainHeight(x, y, z);
    
    if (y > height) {
        // Над поверхностью
        if (y < 64) return blocks::WATER_BLOCK->id;  // Вода ниже уровня моря
        return blocks::AIR_BLOCK->id;                // Воздух
    }
    else if (y > height - 1) {
        // Поверхностный слой
        if (y > 90) return blocks::SNOW_BLOCK->id;   // Снег на вершинах
        if (y > 80) return blocks::COBBLESTONE_BLOCK->id;  // Камень на высоте
        return blocks::GRASS_BLOCK->id;                   // Трава
    }
    else if (y > height - 4) {
        // Подпочвенный слой
        return blocks::DIRT_BLOCK->id;
    }
    else {
        // Глубокие слои
        return blocks::COBBLESTONE_BLOCK->id;
    }
}


int WorldGeneration::randomSeed()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 483647); // distribution in range [1, 6]

    return dist6(rng);
}
