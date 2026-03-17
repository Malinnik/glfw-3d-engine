#include "generation.h"

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include <random>
#include <iostream>

#include "blocks/blocks.h"
#include "blocks/block.h"
#include "world/chunk.h"

float WorldGenerator::getTerrainHeight(int x, int z)
{
    float height = 0.0f;

    float baseFrequency = 0.01f;
    float amplitude = 40.0f;
    float persistance = 0.5f;

    int octaves = 6;
    float freq = baseFrequency;
    float amp = amplitude;

    int seed = WorldGenerator::randomSeed();

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

inline float WorldGenerator::getBiomeAdjustedHeight(int x, int y, int z) {
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

int WorldGenerator::getBlockType(int x, int y, int z)
{
    float height = WorldGenerator::getTerrainHeight(x, z);
    
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

void WorldGenerator::generate(unsigned int* blockIds, int cx, int cy, int cz)
{
    for (int z = 0; z < CHUNK_D; z++)
    {
        for (int x = 0; x < CHUNK_W; x++)
        {
            int real_x = x + cx * CHUNK_W;
            int real_z = z + cx * CHUNK_D;
            int real_y = WorldGenerator::getTerrainHeight(real_x, real_z);

            for (int y = 0; y < CHUNK_H; y++)
            {
                blockIds[(y * CHUNK_D  + z) * CHUNK_W + x] = WorldGenerator::getBlockType(real_x, real_y, real_z);
            }
        }
    }
}

void WorldGenerator::generate_v2(unsigned int* blockIds, int cx, int cy, int cz){
	for (int z = 0; z < CHUNK_D; z++){
		for (int x = 0; x < CHUNK_W; x++){
			int real_x = x + cx * CHUNK_W;
			int real_z = z + cz * CHUNK_D;
			float height = glm::perlin(glm::vec3(real_x*0.0125f,real_z*0.0125f, 0.0f));
			height += glm::perlin(glm::vec3(real_x*0.025f,real_z*0.025f, 0.0f))*0.5f;
			height *= 0.1f;
			height += 0.05f;
			for (int y = 0; y < CHUNK_H; y++){
				int real_y = y + cy * CHUNK_H;
				float noise = height;
				int id = (noise / std::fmax(0.01f, real_y*0.1f + 0.1f)) > 0.1f ? 1 : 0;
				if (real_y <= 2)
					id = 2;

				if (id == 0 && real_y == 14 && height <= 0.01f)
					id = 1;
				blockIds[(y * CHUNK_D + z) * CHUNK_W + x] = id;
			}
		}
	}
}

int WorldGenerator::randomSeed()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 483647); // distribution in range [1, 6]

    return dist6(rng);
}
