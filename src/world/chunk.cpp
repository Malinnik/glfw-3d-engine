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
	for (int z = 0; z < CHUNK_D; z++)
    {
		for (int x = 0; x < CHUNK_W; x++)
        {
			int real_x = x + this->x * CHUNK_W;
			int real_z = z + this->z * CHUNK_D;
			
			for (int y = 0; y < CHUNK_H; y++)
            {
				int real_y = y + this->y * CHUNK_H;
				blocksIds[(y * CHUNK_D + z) * CHUNK_W + x] = WorldGenerator::getBlockType(real_x, real_y, real_z);
			}
		}
	}
}

Chunk::~Chunk()
{
    delete[] blocksIds; 
}

bool Chunk::isEmpty()
{
    int id = -1;
	for (int i = 0; i < CHUNK_BLOCKS; i++){
		if (blocksIds[i] != id){
			if (id != -1)
				return false;
			else
				id = blocksIds[i];
		}
	}
	return true;
}
