#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include "chunk.h"
#include "blocks/block.h"
#include "world/generation.h"

#include "loguru.hpp"
#include "fmt/format.h"

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos)
{
	blocks = new block[CHUNK_BLOCKS];
	
    for (int z = 0; z < CHUNK_D; z++)
    {
		for (int x = 0; x < CHUNK_W; x++)
        {
			int real_x = x + this->x * CHUNK_W;
			int real_z = z + this->z * CHUNK_D;
			
			for (int y = 0; y < CHUNK_H; y++)
            {
				int real_y = y + this->y * CHUNK_H;
				blocks[(y * CHUNK_D + z) * CHUNK_W + x].id = WorldGeneration::getBlockType(real_x, real_y, real_z);
			}
		}
	}
}

Chunk::~Chunk()
{
    delete[] blocks; 
}

inline float Chunk::generate(int x, int y, int z)
{
    return glm::perlin(glm::vec3(x*0.025f, y*0.025f, z*0.025f)); //real_y <= (height) * 60 + 30;
}
